#ifndef _ENGINE_H
#define _ENFINE_H

#ifndef _WINDOWS_
#include <Windows.h>
#endif // _WINDOWS_

#ifndef _MAP_
#include <map>
#endif // _MAP_

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

struct Engine
{
    Engine();
    ~Engine();

    static EngineState GetEngineState() { return m_EngineState; }

    // =================================================//

    static EngineState m_EngineState;
};
#endif // _ENGINE_H



