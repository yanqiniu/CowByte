#include <ctime>

#include "Engine.h"
#include "System.h"
#include "Game.h"
#include "Input.h"

#include "../Render/Window.h"
#include "../Render/Graphics.h"
#include "../SceneGraph/SceneNode.h"

#include <new.h>

EngineState Engine::m_EngineState = EngineState::INVALID;


Engine::Engine(const EngineStartParam& startParam) : 
    Component(),
    m_hInst(startParam.m_hInst)
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

    // Add Systems and Game.
    Window *pWindow = new Window(WindowData(640, 480, m_hInst));
    Graphics *pGraphics = new Graphics(GraphicsData(pWindow));
    Input *pInput = new Input(InputSystemData(pWindow));
    Game* pGame = new Game(GameData(pInput));


    if (!AddSystem(pWindow))
        return false;
    if (!AddSystem(pGraphics))
        return false;
    if (!AddSystem(pInput))
        return false;
    if (!AddSystem(pGame))
        return false;


    //Initialize Systems.
    if (!m_MapSystems[SystemType::SYS_WINDOW]->Initialize())
        return false;
    if (!m_MapSystems[SystemType::SYS_GRAPHICS]->Initialize())
        return false;
    if (!m_MapSystems[SystemType::SYS_INPUT]->Initialize())
        return false;
    if (!m_MapSystems[SystemType::SYS_GAME]->Initialize())
        return false;


    // Subscribe systems to the message bus.
    // NOTICE: the order specified here will be the order they get called when 
    // doing message handling.
    MessageBus::GetEngineBus()->AddSubscriber(m_MapSystems[SystemType::SYS_GAME]);
    MessageBus::GetEngineBus()->AddSubscriber(&SceneNode::RootNode);
    MessageBus::GetEngineBus()->AddSubscriber(m_MapSystems[SystemType::SYS_GRAPHICS]);
    MessageBus::GetEngineBus()->AddSubscriber(m_MapSystems[SystemType::SYS_WINDOW]);
    MessageBus::GetEngineBus()->AddSubscriber(m_MapSystems[SystemType::SYS_INPUT]);

    // Finally, process all the messages queued up during initialization.
    MessageBus::GetEngineBus()->Broadcast();
    MessageBus::GetEngineBus()->SubsHandleMessagesQueueTree();
    return true;
}

int Engine::Draw(GameContext& context)
{
    return true;
}

bool Engine::Update(const GameContext& context)
{
    // Empty function, this might never get called.
    // We have this because Engine inherits from Component
    // which has Update() as a pure virtual.
    // Engine class mosly operate inside Runloop().
    return true;
}

int Engine::ShutDown()
{
    m_EngineState = EngineState::SHUTTINGDOWN;

    for (std::pair<SystemType, System*> pSysPair : m_MapSystems)
    {
        if (!pSysPair.second->Shutdown())
        {
        	DbgWARNING("Failed to shutdown system [%d]", pSysPair.first);
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
    context.dTime = 0;

    std::clock_t timer;
    while (msg.message != WM_QUIT && m_EngineState == EngineState::RUNNING)
    {
        // Start Timing this frame:
        timer = std::clock();

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
        m_MapSystems[SystemType::SYS_INPUT]->UpdateTree(context);

        // Now broadcast all the messages received in the earlier:
        MessageBus::GetEngineBus()->Broadcast();

        // Process all the messages:
        MessageBus::GetEngineBus()->SubsHandleMessagesQueueTree();

        // TODO: Late update.

        // Update deltaTime
        context.dTime = (std::clock() - timer) / (double)CLOCKS_PER_SEC;
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

void Engine::_HandleMessage(CBRefCountPtr<Message> &pMsg)
{

}
