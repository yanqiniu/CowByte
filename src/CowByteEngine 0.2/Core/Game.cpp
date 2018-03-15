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

    return true;
}

bool Game::Update(const GameContext &context)
{
    //m_pMainCamera->UpdateWToCMatrix();

    Vec3 translateAmount(0.5f, 0, 0);
    translateAmount *= context.dTime;
    m_pTestSceneNode->Translate(Vec3(0.5, 0, 0) * context.dTime);
    m_pTestSceneNode->UpdateWorldTransform();

    return true;
}

void Game::_HandleMessage(CBRefCountPtr<Message> &pMsg)
{

}

GameData::GameData() :
    SystemData(SystemType::SYS_INVALID)
{

}
