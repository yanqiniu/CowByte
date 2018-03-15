#ifndef _GAME_H
#define _GAME_H

#include "System.h"
class Camera;

struct GameData : public SystemData
{
    GameData();
};

class Game : public System
{
public:
    Game(const GameData& gameData);
    virtual ~Game();

    bool Initialize();
    bool Update(const GameContext &context);
    void _HandleMessage(CBRefCountPtr<Message> &pMsg) override;
private:
    Camera *m_pMainCamera; // Pointer to current active cam.
    SceneNode *m_pTestSceneNode;
    SceneNode *m_pTestSceneNode2;
};

#endif


