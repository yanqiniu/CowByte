#include "Engine.h"
#include "System.h"
#include "Game.h"

#include "../Render/Window.h"
#include "../Render/Graphics.h"

#include <new.h>

EngineState Engine::m_EngineState = EngineState::INVALID;

Engine::Engine()
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

    Game* pGame = CreateGame();
    if (!pGame)
        return false;
    context.pEngineMessageBus->AddSubscriber(pGame);
    

    // Add Systems.
    Window *window = new Window(WindowData(640, 480));
    Graphics *graphics = new Graphics(GraphicsData(window));
    if (!AddSystem(window))
        return false;
    if (!AddSystem(graphics))
        return false;

    //Initialize Systems.
    if (!m_MapSystems[SystemType::SYS_WINDOW]->Initialize())
        return false;
    if (!m_MapSystems[SystemType::SYS_GRAPHICS]->Initialize())
        return false;

    // Subscribe systems to the message bus.
    context.pEngineMessageBus->AddSubscriber(m_MapSystems[SystemType::SYS_WINDOW]);
    context.pEngineMessageBus->AddSubscriber(m_MapSystems[SystemType::SYS_GRAPHICS]);

    return true;
}

int Engine::Draw(GameContext& context)
{
    return true;
}

int Engine::Update(GameContext& context)
{
    for (std::pair<SystemType, System*> pSys : m_MapSystems)
    {
        if(!pSys.second)
            continue;

        pSys.second->Update(context);
    }
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
    context.pEngineMessageBus = &m_EngineMessageBus;
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

        this->Update(context);
        this->Draw(context);
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
