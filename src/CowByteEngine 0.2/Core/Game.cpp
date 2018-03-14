#include "Game.h"
#include "Message.h"
#include "../SceneGraph/SceneNode.h"
#include "../SceneGraph/Camera.h"
#include "../Render/MeshInstance.h"



Game::Game(const GameData& gameData) :
    System(SystemType::SYS_GAME),
    m_pMainCamera(nullptr)
{
}

Game::~Game()
{
}

// Call this after systems have been initialized.
bool Game::Initialize()
{
    // Initialize Camera.
    m_pMainCamera = new Camera();
    SceneNode *pCameraSceneNode = SceneNode::CreateSceneNodeThenAttach(&SceneNode::RootNode);
    m_pMainCamera->AttachTo_SceneNode_Parent(pCameraSceneNode);


    // TEMP: Create Mesh in the scene.
    SceneNode *cubeSceneNode = SceneNode::CreateSceneNodeThenAttach(&SceneNode::RootNode);
    MeshInstance *cube1 = new MeshInstance("cube.mesha");
    PostMessage(Msg_RegisterDrawbleMeshInst(cube1));

    return true;
}

bool Game::Update()
{
    m_pMainCamera->UpdateWToCMatrix();

    return true;
}

GameData::GameData() :
    SystemData(SystemType::SYS_INVALID)
{

}
