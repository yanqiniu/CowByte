#ifndef _GAME_H
#define _GAME_H

#include "System.h"
class Camera;

struct GameData : public SystemData
{
    GameData();
};

// A test object. Likely this will be made into a GameObject class.
struct Cube
{
    SceneNode *m_pSceneNode;
    Cube();

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

    // Test game objects
    Cube *m_pCube0;
    Cube *m_pCube1;
};

#endif


