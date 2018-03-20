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

    m_pCube0->m_pSceneNode->Translate(0.0f, 3.0f, 0.0f);
    m_pPlane->m_pSceneNode->Translate(0.0f, -3.0f, 0.0f);
    

    // Create game camera.
    m_pGameCamera = new Camera((float)800 / 600, 40.0f, 0.01f, 1000.0f);
    SceneNode *cameraSceneNode = SceneNode::CreateSceneNodeThenAttach(&SceneNode::RootNode);
    m_pGameCamera->AttachTo_SceneNode_Parent(cameraSceneNode);
    cameraSceneNode->Translate(Vec3(0, 0, -10.0f));

    // Set the game camera as main.
    CBRefCountPtr<Message> msgPtr = Msg_SetMainCamera::Create();
    MESSAGE_FROM_PTR(msgPtr, Msg_SetMainCamera)->m_pCamera = m_pGameCamera;
    CBMessaging::PostQueuedMessage(msgPtr, MessageBus::GetEngineBus());

    return true;
}

bool Game::Update(const GameContext &context)
{
    m_pCube0->m_pSceneNode->Rotate(Vec3(0, 1, 0), 15.0f * context.dTime);

    m_pCube1->m_pSceneNode->Rotate(Vec3(0, 1, 0), -30.0f * context.dTime);

    Matrix4x4 mat = m_pGameCamera->GetParentSceneNode()->GetWorldTransform();
    Vec3 up = mat.Up();
    Vec3 rt = mat.Right();
    Vec3 fr = mat.Front();

    if (m_pInput->GetKeyHeld(KeyCodes::KEY_D))
    {
        m_pGameCamera->GetParentSceneNode()->RotateLocal(Vec3::Up(), 30.0f * context.dTime);
    }
    else if (m_pInput->GetKeyHeld(KeyCodes::KEY_A))
    {
        m_pGameCamera->GetParentSceneNode()->RotateLocal(Vec3::Up(), -30.0f * context.dTime);
    }

    if (m_pInput->GetKeyHeld(KeyCodes::KEY_W))
    {
        m_pGameCamera->GetParentSceneNode()->RotateLocal(rt, -30.0f * context.dTime);
    }
    else if (m_pInput->GetKeyHeld(KeyCodes::KEY_S))
    {
        m_pGameCamera->GetParentSceneNode()->RotateLocal(rt, 30.0f * context.dTime);
    }
    else if (m_pInput->GetKeyHeld(KeyCodes::KEY_SPACE))
    {
        m_pGameCamera->GetParentSceneNode()->Translate(fr * context.dTime);
    }
    else if (m_pInput->GetKeyHeld(KeyCodes::KEY_LSHIFT))
    {
        m_pGameCamera->GetParentSceneNode()->Translate(fr * (-context.dTime));
    }
    if (m_pInput->GetKeyUp(KeyCodes::KEY_F))
    {
        m_pGameCamera->GetParentSceneNode()->LookAt(*(m_pCube0->m_pSceneNode), Vec3::Up());
    }
    if (m_pInput->GetKeyUp(KeyCodes::KEY_G))
    {
        m_pGameCamera->GetParentSceneNode()->RotateLocal(rt, -16.699244f);
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
