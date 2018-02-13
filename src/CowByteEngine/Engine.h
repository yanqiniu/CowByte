#ifndef _ENGINE_H
#define _ENFINE_H

#ifndef _WINDOWS_
#include <Windows.h>
#endif // _WINDOWS_

#ifndef _MAP_
#include <map>
#endif // _MAP_

enum SystemType;
class System;
class Game;
struct Context;

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

class Engine
{
public:
    Engine();
    ~Engine();

	int Initialize();
	int Draw(const Context& context);
	int Update(const Context& context);
	int ShutDown();
	int RunLoop();

	// Add a core system to the engine.
	int AddSystem(System* sys);
	// Retrieve a core system.
	template<typename T>
	T* GetSystem(SystemType sysType);

	// Create the game instance.
	Game* CreateGame();

	//void* operator new(size_t size);
	//void operator delete(void* pDelete);

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
		// Logger::Log("System invalid!");
		return nullptr;
	}
	return pSys;

}

#endif // _ENGINE_H



