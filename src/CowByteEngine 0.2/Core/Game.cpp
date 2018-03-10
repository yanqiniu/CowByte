#include "Game.h"



Game::Game(const GameData& gameData) :
    System(SystemType::SYS_GAME)
{
}

Game::~Game()
{
}

GameData::GameData() :
    SystemData(SystemType::SYS_INVALID)
{

}
