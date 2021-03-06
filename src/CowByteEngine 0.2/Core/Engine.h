#ifndef _ENGINE_H
#define _ENFINE_H

#include <Windows.h>
#include <map>
#include "../Utils/CBDebug.h"
#include "../Messaging/CBMessaging.h"
#include "Component.h"

enum SystemType;
class System;
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

struct EngineStartParam
{
    HINSTANCE m_hInst;
};

class Engine : public Component
{
public:
    Engine(const EngineStartParam& startParam);
    ~Engine();

    int  Initialize(GameContext &context);
    bool Update(const GameContext& context);
    bool  Shutdown();
    int  RunLoop();

    // Add a core system to the engine.
    int AddSystem(System* sys);
    // Retrieve a core system.
    template<typename T>
    T* GetSystem(SystemType sysType);

    static EngineState GetEngineState() { return m_EngineState; }
    void _HandleMessage(CBRefCountPtr<Message> &pMsg) override;

    // =================================================//

private:
    std::map<SystemType, System*> m_MapSystems;
    static EngineState m_EngineState;
    HINSTANCE m_hInst;
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



