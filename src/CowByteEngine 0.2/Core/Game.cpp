#include "Game.h"
#include "Input.h"
#include "../Messaging/CBMessaging.h"
#include "../SceneGraph/SceneNode.h"
#include "../SceneGraph/Camera.h"
#include "../Render/GeometryCPU/MeshInstance.h"
#include "../Utils/CBRefCountPtr.h"
#include "../SceneGraph/Light.h"

#define DEFINE_DEBUG_GAME_OBJECT(ClassName, MeshFile)                                   \
ClassName::ClassName()                                                                  \
{                                                                                       \
    m_pSceneNode = SceneNode::CreateSceneNodeThenAttach(&SceneNode::RootNode);          \
    MeshInstance *pMeshInst = new MeshInstance(MeshFile);                               \
    pMeshInst->AttachTo_SceneNode_Parent(m_pSceneNode);                                 \
    CBRefCountPtr<Message> msgPtr = Msg_RegisterDrawbleMeshInst::Create();              \
    MESSAGE_FROM_PTR(msgPtr, Msg_RegisterDrawbleMeshInst)->m_MeshInstPtr = pMeshInst;   \
    CBMessaging::PostQueuedMessage(msgPtr, MessageBus::GetEngineBus());                 \
} 

DEFINE_DEBUG_GAME_OBJECT(Cube, "cube.mesha");
DEFINE_DEBUG_GAME_OBJECT(Plane, "plane.mesha");
DEFINE_DEBUG_GAME_OBJECT(Torus, "torus.mesha");
DEFINE_DEBUG_GAME_OBJECT(Cow, "cow.mesha");
DEFINE_DEBUG_GAME_OBJECT(Sphere, "sphere.mesha");

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
    DbgAssert(m_pInput != nullptr, "Null input used by game!");

    // Game objects setup.
    //m_pCube0 = new Cube();
    m_pCube1 = new Cube();
    m_pPlane = new Plane();
    //m_pTorus = new Torus();
    //m_pCow = new Cow();
    m_pSphere = new Sphere();
    //m_pCube0->m_pSceneNode->Translate(0.0f, 3.0f, 0.0f);
    //m_pCube0->m_pSceneNode->Scale(2.0f);
    m_pCube1->m_pSceneNode->Scale(1.2f);
    m_pCube1->m_pSceneNode->Translate(-3.0f, 1.0f, 0.0f);
    //m_pPlane->m_pSceneNode->Translate(0.0f, -3.0f, 0.0f);
    m_pPlane->m_pSceneNode->Scale(10.0f);
    //m_pTorus->m_pSceneNode->Translate(0.0f, 0.0f, 5.0f);
    //m_pTorus->m_pSceneNode->Scale(10.0f);
    m_pSphere->m_pSceneNode->Translate(-3, 3.0f, -3.0f);

    // Create lighting
    m_pDirLight = new Light();
    SceneNode* pDirLightSN = SceneNode::CreateSceneNodeThenAttach(&SceneNode::RootNode);
    m_pDirLight->AttachTo_SceneNode_Parent(pDirLightSN);
    pDirLightSN->LookAt(Vec3(1.0f, -1.0f, 1.0f), Vec3::Up());
    m_pDirLight->InitializeDirectional(CBColor(0.3f, 0.3f, 0.3f, 1.0f), true);
    m_pDirLight->RegisterLight();

    m_pPtLight = new Light();
    SceneNode* pPtLightSN = SceneNode::CreateSceneNodeThenAttach(&SceneNode::RootNode);
    pPtLightSN->Translate(Vec3(0.0f, 10.0f, 0.0f));
    m_pPtLight->AttachTo_SceneNode_Parent(pPtLightSN);
    m_pPtLight->InitializePoint(CBColor(1.0f, 1.0f, 1.0f, 1.0f), 30.0f);
    m_pPtLight->RegisterLight();


    // Create game camera.
    m_pGameCamera = new Camera((float)800 / 600, 40.0f, 0.01f, 100.0f);
    SceneNode *cameraSceneNode = SceneNode::CreateSceneNodeThenAttach(&SceneNode::RootNode);
    m_pGameCamera->AttachTo_SceneNode_Parent(cameraSceneNode);
    cameraSceneNode->RotateLocal(m_pGameCamera->GetParentSceneNode()->GetWorldTransform().Up(), 45.0f);
    cameraSceneNode->RotateLocal(m_pGameCamera->GetParentSceneNode()->GetWorldTransform().Right(), 25.0f);
    cameraSceneNode->Translate(Vec3(-15, 10.0f, -15.0f));

    // Set the game camera as main.
    CBRefCountPtr<Message> msgPtr = Msg_SetMainCamera::Create();
    MESSAGE_FROM_PTR(msgPtr, Msg_SetMainCamera)->m_pCamera = m_pGameCamera;
    CBMessaging::PostQueuedMessage(msgPtr, MessageBus::GetEngineBus());

    return true;
}

bool Game::Update(const GameContext &context)
{
    NaviCameraUpdate(context);

    //m_pCow->m_pSceneNode->Rotate(Vec3(0, 1, 0), 15.0f * context.dTime);
    m_pCube1->m_pSceneNode->RotateLocal(Vec3(0, 1, 0), -30.0f * context.dTime);
    //m_pDirLight->GetParentSceneNode()->RotateLocal(Vec3::Up(), 30.0f * context.dTime);
    return true;
}

void Game::_HandleMessage(CBRefCountPtr<Message> &pMsg)
{

}

// Navigational camera update. This is made for debugging.
// Remove for actual game.
void Game::NaviCameraUpdate(const GameContext &context)
{
    Vec3 up = m_pGameCamera->GetParentSceneNode()->GetWorldTransform().Up();
    Vec3 rt = m_pGameCamera->GetParentSceneNode()->GetWorldTransform().Right();
    Vec3 fr = m_pGameCamera->GetParentSceneNode()->GetWorldTransform().Front();

#define CAM_MOVE_SPEED 15.0f
#define CAM_ROTATE_SPEED 30.0f
#define CAM_RISE_SPEED 15.0f

    // WASD movement:
    if (m_pInput->GetKeyHeld(KeyCodes::KEY_W))
        m_pGameCamera->GetParentSceneNode()->Translate(fr * (CAM_MOVE_SPEED) * context.dTime);
    else if (m_pInput->GetKeyHeld(KeyCodes::KEY_S))
        m_pGameCamera->GetParentSceneNode()->Translate(fr * (-CAM_MOVE_SPEED) * context.dTime);
    if (m_pInput->GetKeyHeld(KeyCodes::KEY_D))
        m_pGameCamera->GetParentSceneNode()->Translate(rt * (CAM_MOVE_SPEED) * context.dTime);
    else if (m_pInput->GetKeyHeld(KeyCodes::KEY_A))
        m_pGameCamera->GetParentSceneNode()->Translate(rt * (-CAM_MOVE_SPEED) * context.dTime);

    // Arrows rotation:
    if (m_pInput->GetKeyHeld(KeyCodes::KEY_LEFT))
        m_pGameCamera->GetParentSceneNode()->RotateLocal(Vec3::Up(), -CAM_ROTATE_SPEED * context.dTime);
    else if (m_pInput->GetKeyHeld(KeyCodes::KEY_RIGHT))
        m_pGameCamera->GetParentSceneNode()->RotateLocal(Vec3::Up(), CAM_ROTATE_SPEED * context.dTime);
    if (m_pInput->GetKeyHeld(KeyCodes::KEY_UP))
        m_pGameCamera->GetParentSceneNode()->RotateLocal(rt, -CAM_ROTATE_SPEED * context.dTime);
    else if (m_pInput->GetKeyHeld(KeyCodes::KEY_DOWN))
        m_pGameCamera->GetParentSceneNode()->RotateLocal(rt, CAM_ROTATE_SPEED * context.dTime);

    // Space: rise, LShift: fall:
    if (m_pInput->GetKeyHeld(KeyCodes::KEY_SPACE))
        m_pGameCamera->GetParentSceneNode()->Translate(Vec3::Up() * (CAM_RISE_SPEED) * context.dTime);
    else if (m_pInput->GetKeyHeld(KeyCodes::KEY_LSHIFT))
        m_pGameCamera->GetParentSceneNode()->Translate(Vec3::Up() * (-CAM_RISE_SPEED) * context.dTime);

#undef CAM_MOVE_SPEED
#undef CAM_ROTATE_SPEED
#undef CAM_RISE_SPEED

}

GameData::GameData(Input* input) :
    SystemData(SystemType::SYS_INVALID),
    m_pInput(input)
{

}
