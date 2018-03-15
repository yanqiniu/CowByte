#ifndef _ENGINE_H
#define _ENFINE_H

#include <Windows.h>
#include <map>
#include "../Utils/CBDebug.h"
#include "MessageBus.h"
#include "Component.h"

enum SystemType;
class System;
class Game;
class MessageBus;
struct GameContext;

//Enumeration
enum EngineState
{
    INVALID,
    CONSTRUCTING,
    INITIALIZING,
    RUNNING,
    SHUTTINGDOWN,
    DESTROYING
};

class Engine : public Component
{
public:
    Engine();
    ~Engine();

    int Initialize(GameContext &context);
    int Draw(GameContext& context);
    int Update(GameContext& context);
    int ShutDown();
    int RunLoop();

    // Add a core system to the engine.
    int AddSystem(System* sys);
    // Retrieve a core system.
    template<typename T>
    T* GetSystem(SystemType sysType);

    // Create the game instance.
    Game* CreateGame();

    static EngineState GetEngineState() { return m_EngineState; }

    // =================================================//

private:
    std::map<SystemType, System*> m_MapSystems;
    static EngineState m_EngineState;
};

template<typename T>
T* Engine::GetSystem(SystemType sysType)
{
    T* pSys = static_cast<T*>(m_MapSystems[sysType]);
    if (!pSys)
    {
        DbgWARNING("System invalid!");
        return nullptr;
    }
    return pSys;

}

#endif // _ENGINE_H



