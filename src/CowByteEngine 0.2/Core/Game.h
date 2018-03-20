#ifndef _GAME_H
#define _GAME_H

#include "System.h"

class Camera;
class Input;

struct GameData : public SystemData
{
    GameData(Input* input);

    Input *m_pInput;
};

// A test object. Likely this will be made into a GameObject class.
struct Cube
{
    SceneNode *m_pSceneNode;
    Cube();
};

// A test object. Likely this will be made into a GameObject class.
struct Plane
{
    SceneNode *m_pSceneNode;
    Plane();
};


// This will be polished into a general purpose Game class that
// custom Game class can inherit from.
// Right now it's more of a testing entry point.
class Game : public System
{
public:
    Game(const GameData& gameData);
    virtual ~Game();

    bool Initialize();
    bool Update(const GameContext &context);
    void _HandleMessage(CBRefCountPtr<Message> &pMsg) override;
private:
    void NaviCameraUpdate(const GameContext &context);

    Input  *m_pInput; // This couples Game and Input and is not ideal. But this does provide Game with different input source.
    Camera *m_pGameCamera; // Pointer to current active cam.

    // Test game objects
    Cube *m_pCube0;
    Cube *m_pCube1;
    Plane *m_pPlane;
};

#endif


