#include "Game.h"
#include "Message.h"
#include "MessageBus.h"
#include "../SceneGraph/SceneNode.h"
#include "../SceneGraph/Camera.h"
#include "../Render/MeshInstance.h"
#include "../Utils/CBRefCountPtr.h"



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
    //// Initialize Camera.
    //m_pMainCamera = new Camera();
    //SceneNode *pCameraSceneNode = SceneNode::CreateSceneNodeThenAttach(&SceneNode::RootNode);
    //m_pMainCamera->AttachTo_SceneNode_Parent(pCameraSceneNode);


    // TEMP: Create Mesh in the scene.
    m_pTestSceneNode = SceneNode::CreateSceneNodeThenAttach(&SceneNode::RootNode);
    MeshInstance *cube1 = new MeshInstance("cube.mesha");
    cube1->AttachTo_SceneNode_Parent(m_pTestSceneNode);

    CBRefCountPtr<Message> msgPtr = Message::Create(MessageType::MsgType_RegisterDrawbleMeshInstance);
    static_cast<Msg_RegisterDrawbleMeshInst*>(msgPtr.Get())->m_MeshInstPtr = cube1;
    PostMessage(msgPtr, MessageBus::GetEngineBus());

    // TEMP: Create A Second in the scene.
    m_pTestSceneNode2 = SceneNode::CreateSceneNodeThenAttach(&SceneNode::RootNode);
    MeshInstance *cube2 = new MeshInstance("cube.mesha");
    cube2->AttachTo_SceneNode_Parent(m_pTestSceneNode2);

    CBRefCountPtr<Message> msgPtr2 = Message::Create(MessageType::MsgType_RegisterDrawbleMeshInstance);
    static_cast<Msg_RegisterDrawbleMeshInst*>(msgPtr2.Get())->m_MeshInstPtr = cube2;
    PostMessage(msgPtr2, MessageBus::GetEngineBus());


    return true;
}

bool Game::Update(const GameContext &context)
{
    //m_pMainCamera->UpdateWToCMatrix();

    m_pTestSceneNode->Translate(Vec3(0.5f, 0, 0) * context.dTime);
    m_pTestSceneNode->UpdateWorldTransform();

    m_pTestSceneNode2->Translate(Vec3(0, 0.5f, 0) * context.dTime);
    m_pTestSceneNode2->UpdateWorldTransform();

    return true;
}

void Game::_HandleMessage(CBRefCountPtr<Message> &pMsg)
{

}

GameData::GameData() :
    SystemData(SystemType::SYS_INVALID)
{

}
