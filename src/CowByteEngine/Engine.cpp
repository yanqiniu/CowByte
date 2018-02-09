#include "Engine.h"

EngineState Engine::m_EngineState = EngineState::INVALID;

Engine::Engine()
{
	m_EngineState = EngineState::CONSTRUCTING;
}


Engine::~Engine()
{
}

int Engine::Initialize()
{

}

int Engine::Draw()
{

}

int Engine::Update()
{

}

int Engine::ShutDown()
{

}

int Engine::RunLoop()
{

}

int Engine::AddSystem(System* sys)
{

}

Game* Engine::CreateGame()
{

}
