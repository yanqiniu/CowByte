#ifndef _GAME_H
#define _GAME_H

#include "System.h"

struct GameData : public SystemData
{

};

class Game : public System
{
public:
	Game(const GameData& gameData);
	virtual ~Game();
};

#endif


