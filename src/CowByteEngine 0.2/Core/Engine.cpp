#include "Engine.h"
#include "System.h"
#include "Game.h"

#include "../Render/Window.h"
#include "../Render/Graphics.h"
#include "../SceneGraph/SceneNode.h"

#include <new.h>

EngineState Engine::m_EngineState = EngineState::INVALID;

Engine::Engine() :
    Component()
{
    m_EngineState = EngineState::CONSTRUCTING;
}


Engine::~Engine()
{
    m_EngineState = EngineState::DESTROYING;
}

int Engine::Initialize(GameContext &context)
{
    m_EngineState = EngineState::INITIALIZING;
    // Construct Engine bus.
    CB::InitializeEngineBus();

    // Add Game.
    Game* pGame = CreateGame();
    if (!pGame)
        return false;

    // Add Systems.
    Window *pWindow = new Window(WindowData(640, 480));
    Graphics *pGraphics = new Graphics(GraphicsData(pWindow));
    if (!AddSystem(pWindow))
        return false;
    if (!AddSystem(pGraphics))
        return false;

    //Initialize Systems.
    if (!m_MapSystems[SystemType::SYS_WINDOW]->Initialize())
        return false;
    if (!m_MapSystems[SystemType::SYS_GRAPHICS]->Initialize())
        return false;
    if (!m_MapSystems[SystemType::SYS_GAME]->Initialize())
        return false;


    // Subscribe systems to the message bus.
    MessageBus::GetEngineBus()->AddSubscriber(m_MapSystems[SystemType::SYS_WINDOW]);
    MessageBus::GetEngineBus()->AddSubscriber(m_MapSystems[SystemType::SYS_GRAPHICS]);
    MessageBus::GetEngineBus()->AddSubscriber(m_MapSystems[SystemType::SYS_GAME]);

    MessageBus::GetEngineBus()->AddSubscriber(&SceneNode::RootNode);



    return true;
}

int Engine::Draw(GameContext& context)
{
    return true;
}

bool Engine::Update(const GameContext& context)
{


    return true;
}

int Engine::ShutDown()
{
    m_EngineState = EngineState::SHUTTINGDOWN;

    for (std::pair<SystemType, System*> pSysPair : m_MapSystems)
    {
        if (!pSysPair.second->Shutdown())
        {
        	DbgWARNING("Failed to shurdown system [%d]", pSysPair.first);
        	continue;
        }

        delete (pSysPair.second);
    }

    return true;
}

int Engine::RunLoop()
{
    GameContext context;
    if (!this->Initialize(context))
        return 0;

    srand(GetTickCount());

    MSG msg = {};

    m_EngineState = EngineState::RUNNING;


    while (msg.message != WM_QUIT && m_EngineState == EngineState::RUNNING)
    {
        //CheckResize();
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Update each system:
        m_MapSystems[SystemType::SYS_GAME]->UpdateTree(context); // Update game first.
        SceneNode::RootNode.UpdateTree(context); // Update scene.
        m_MapSystems[SystemType::SYS_GRAPHICS]->UpdateTree(context);
        m_MapSystems[SystemType::SYS_WINDOW]->UpdateTree(context);

        // Now broadcast all the messages received in the earlier:
        MessageBus::GetEngineBus()->Broadcast();

        // Process all the messages:
        m_MapSystems[SystemType::SYS_GAME]->HandleMessagesQueueTree(); // Update game first.
        SceneNode::RootNode.HandleMessagesQueueTree(); // Update scene.
        m_MapSystems[SystemType::SYS_GRAPHICS]->HandleMessagesQueueTree();
        m_MapSystems[SystemType::SYS_WINDOW]->HandleMessagesQueueTree();


        // TODO: Late update.
    }

    DbgINFO("Ending the program...");
    //Logger::WriteLogToFile();

    if (!this->ShutDown())
        return 0;

    return msg.wParam;
}

int Engine::AddSystem(System* pSys)
{
    auto element = m_MapSystems.insert(std::make_pair(pSys->GetType(), pSys));
    if (element.second)
        return true;

    return false;
}

Game* Engine::CreateGame()
{
    if (!AddSystem(new Game(GameData())))
        return nullptr;

    Game* game = GetSystem<Game>(SystemType::SYS_GAME);
    if (!game)
        return nullptr;
    //if (!game->Initialize())
    //	return nullptr;

    return game;
}

void Engine::_HandleMessage(CBRefCountPtr<Message> pMsg)
{

}
