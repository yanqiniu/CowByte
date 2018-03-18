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
    CBRefCountPtr<Message> msgPtr = Msg_RegisterDrawbleMeshInst::Create();
    MESSAGE_FROM_PTR(msgPtr, Msg_RegisterDrawbleMeshInst)->m_MeshInstPtr = cube1;
    CBMessaging::PostQueuedMessage(msgPtr, MessageBus::GetEngineBus());
}

Plane::Plane()
{
    // Create MeshInst
    m_pSceneNode = SceneNode::CreateSceneNodeThenAttach(&SceneNode::RootNode);
    MeshInstance *plane = new MeshInstance("plane.mesha");
    plane->AttachTo_SceneNode_Parent(m_pSceneNode);

    // Notify the mesh manager...
    CBRefCountPtr<Message> msgPtr = Msg_RegisterDrawbleMeshInst::Create();
    MESSAGE_FROM_PTR(msgPtr, Msg_RegisterDrawbleMeshInst)->m_MeshInstPtr = plane;
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
    m_pPlane = new Plane();
    m_pPlane->m_pSceneNode->Translate(0.0f, -3.0f, 0.0f);
    m_pPlane->m_pSceneNode->UpdateWorldTransform();
    

    // Create game camera.
    m_pGameCamera = new Camera((float)800 / 600, 0.698131f, 0.01f, 1000.0f);
    SceneNode *cameraSceneNode = SceneNode::CreateSceneNodeThenAttach(&SceneNode::RootNode);
    m_pGameCamera->AttachTo_SceneNode_Parent(cameraSceneNode);
    cameraSceneNode->Translate(Vec3(0, 0, -5.0f));
    cameraSceneNode->UpdateWorldTransform();

    // Set the game camera as main.
    CBRefCountPtr<Message> msgPtr = Msg_SetMainCamera::Create();
    MESSAGE_FROM_PTR(msgPtr, Msg_SetMainCamera)->m_pCamera = m_pGameCamera;
    CBMessaging::PostQueuedMessage(msgPtr, MessageBus::GetEngineBus());

    return true;
}

bool Game::Update(const GameContext &context)
{
    //m_pMainCamera->UpdateWToCMatrix();

    if (m_pInput->GetKeyHeld(KeyCodes::KEY_D))
    {
        m_pGameCamera->GetParentSceneNode()->Translate(Vec3(1.0f, 0.0f, 0.0f) * context.dTime);
        m_pGameCamera->GetParentSceneNode()->UpdateWorldTransform();

        //m_pCube0->m_pSceneNode->Translate(Vec3(0.5f, 0, 0) * context.dTime);
        //m_pCube0->m_pSceneNode->UpdateWorldTransform();

        //m_pCube1->m_pSceneNode->Translate(Vec3(0, 0.5f, 0) * context.dTime);
        //m_pCube1->m_pSceneNode->UpdateWorldTransform();
    }
    else if (m_pInput->GetKeyHeld(KeyCodes::KEY_A))
    {
        m_pGameCamera->GetParentSceneNode()->Translate(Vec3(-1.0f, 0.0f, 0.0f) * context.dTime);
        m_pGameCamera->GetParentSceneNode()->UpdateWorldTransform();

        //m_pCube0->m_pSceneNode->Translate(Vec3(-0.5f, 0, 0) * context.dTime);
        //m_pCube0->m_pSceneNode->UpdateWorldTransform();

        //m_pCube1->m_pSceneNode->Translate(Vec3(0, -0.5f, 0) * context.dTime);
        //m_pCube1->m_pSceneNode->UpdateWorldTransform();
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
