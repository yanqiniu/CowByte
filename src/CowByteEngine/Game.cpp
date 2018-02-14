#include "Game.h"



Game::Game(const GameData& gameData) :
    System(gameData)
{
    m_SystemType = SystemType::SYS_GAME;
}

Game::~Game()
{
}

GameData::GameData() :
    SystemData(SystemType::SYS_GAME)
{

}
