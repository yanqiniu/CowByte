#include "Game.h"
#include "Input.h"
#include "../Messaging/CBMessaging.h"
#include "../SceneGraph/SceneNode.h"
#include "../SceneGraph/Camera.h"
#include "../Render/MeshInstance.h"
#include "../Utils/CBRefCountPtr.h"



Cube::Cube()
{
    // Create MeshInst
    m_pSceneNode = SceneNode::CreateSceneNodeThenAttach(&SceneNode::RootNode);
    MeshInstance *cube1 = new MeshInstance("cube.mesha");
    cube1->AttachTo_SceneNode_Parent(m_pSceneNode);

    // Notify the mesh manager...
    CBRefCountPtr<Message> msgPtr = Message::Create(MessageType::MsgType_RegisterDrawbleMeshInstance);
    static_cast<Msg_RegisterDrawbleMeshInst*>(msgPtr.Get())->m_MeshInstPtr = cube1;
    CBMessaging::PostQueuedMessage(msgPtr, MessageBus::GetEngineBus());
}


Game::Game(const GameData& gameData) :
    System(SystemType::SYS_GAME),
    m_pGameCamera(nullptr),
    m_pInput(gameData.m_pInput)
{
}

Game::~Game()
{
}

// Call this after systems have been initialized.
bool Game::Initialize()
{
    //// Initialize Camera.
    //m_pMainCamera = new Camera();
    //SceneNode *pCameraSceneNode = SceneNode::CreateSceneNodeThenAttach(&SceneNode::RootNode);
    //m_pMainCamera->AttachTo_SceneNode_Parent(pCameraSceneNode);
    DbgAssert(m_pInput != nullptr, "Null input used by game!");

    m_pCube0 = new Cube();
    m_pCube1 = new Cube();

    // Create game camera.
    m_pGameCamera = new Camera((float)800 / 600, 0.698131f, 0.01f, 1000.0f);
    SceneNode *cameraSceneNode = SceneNode::CreateSceneNodeThenAttach(&SceneNode::RootNode);
    m_pGameCamera->AttachTo_SceneNode_Parent(cameraSceneNode);

    // Set the game camera as main.
    CBRefCountPtr<Message> msgPtr = Message::Create(MessageType::MsgType_SetMainCamera);
    static_cast<Msg_SetMainCamera*>(msgPtr.Get())->m_pCamera = m_pGameCamera;
    CBMessaging::PostQueuedMessage(msgPtr, MessageBus::GetEngineBus());

    return true;
}

bool Game::Update(const GameContext &context)
{
    //m_pMainCamera->UpdateWToCMatrix();

    if (m_pInput->GetKeyHeld(KeyCodes::KEY_D))
    {
        m_pCube0->m_pSceneNode->Translate(Vec3(0.5f, 0, 0) * context.dTime);
        m_pCube0->m_pSceneNode->UpdateWorldTransform();

        m_pCube1->m_pSceneNode->Translate(Vec3(0, 0.5f, 0) * context.dTime);
        m_pCube1->m_pSceneNode->UpdateWorldTransform();
    }
    else if (m_pInput->GetKeyHeld(KeyCodes::KEY_A))
    {
        m_pCube0->m_pSceneNode->Translate(Vec3(-0.5f, 0, 0) * context.dTime);
        m_pCube0->m_pSceneNode->UpdateWorldTransform();

        m_pCube1->m_pSceneNode->Translate(Vec3(0, -0.5f, 0) * context.dTime);
        m_pCube1->m_pSceneNode->UpdateWorldTransform();
    }

    if (m_pInput->GetKeyHeld(KeyCodes::KEY_W))
    {
        m_pGameCamera->GetParentSceneNode()->Translate(Vec3(0.0f, 0.0f, 1.0f) * context.dTime);
        m_pGameCamera->GetParentSceneNode()->UpdateWorldTransform();
    }
    else if (m_pInput->GetKeyHeld(KeyCodes::KEY_S))
    {
        m_pGameCamera->GetParentSceneNode()->Translate(Vec3(0.0f, 0.0f, -1.0f) * context.dTime);
        m_pGameCamera->GetParentSceneNode()->UpdateWorldTransform();
    }

    return true;
}

void Game::_HandleMessage(CBRefCountPtr<Message> &pMsg)
{

}

GameData::GameData(Input* input) :
    SystemData(SystemType::SYS_INVALID),
    m_pInput(input)
{

}
