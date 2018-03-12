#include "Game.h"
#include "../SceneGraph/SceneNode.h"
#include "../SceneGraph/Camera.h"



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
    // SceneNode *cubeSceneNode = SceneNode::CreateSceneNodeThenAttach(&SceneNode::RootNode);
    // cubeSceneNode->AddMeshInstance();
    return true;
}

bool Game::Update()
{
    m_pMainCamera->UpdateWToCMatrix();
}

GameData::GameData() :
    SystemData(SystemType::SYS_INVALID)
{

}
