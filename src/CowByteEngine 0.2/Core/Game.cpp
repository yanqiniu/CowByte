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
    m_pMainCamera(nullptr),
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


    return true;
}

bool Game::Update(const GameContext &context)
{
    //m_pMainCamera->UpdateWToCMatrix();

    if (m_pInput->GetKeyHeld(KeyCodes::KEY_W))
    {
        m_pCube0->m_pSceneNode->Translate(Vec3(0.5f, 0, 0) * context.dTime);
        m_pCube0->m_pSceneNode->UpdateWorldTransform();

        m_pCube1->m_pSceneNode->Translate(Vec3(0, 0.5f, 0) * context.dTime);
        m_pCube1->m_pSceneNode->UpdateWorldTransform();
    }
    else if (m_pInput->GetKeyHeld(KeyCodes::KEY_S))
    {
        m_pCube0->m_pSceneNode->Translate(Vec3(-0.5f, 0, 0) * context.dTime);
        m_pCube0->m_pSceneNode->UpdateWorldTransform();

        m_pCube1->m_pSceneNode->Translate(Vec3(0, -0.5f, 0) * context.dTime);
        m_pCube1->m_pSceneNode->UpdateWorldTransform();
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
