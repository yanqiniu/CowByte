#include "Component.h"
#include "MessageBus.h"
#include "../Utils/CBQueue.h"
#include "../Utils/CBDebug.h"
#include "../SceneGraph/SceneNode.h"

Component::Component() :
    m_pMessageBus(nullptr),
    m_MessageQueue(),
    m_Components(4),
    m_bIsActive(true),
    m_pParentComponent(nullptr),
    m_pParentSceneNode(nullptr),
    m_CompType(CompType::DEFAULT)
{

}

Component::~Component()
{

}

void Component::SetMessageBus(MessageBus *mBus)
{
    if (mBus == nullptr)
    {
        DbgWARNING("Trying to subscribe to null message bus!");
        return;
    }
    m_pMessageBus = mBus;
}

void Component::AcceptMessage(const Message &msg)
{
    m_MessageQueue.Enqueue(msg);
}

void Component::PostMessage(const Message &msg)
{
    if (m_pMessageBus == nullptr)
    {
        DbgWARNING("Trying to post to null message bus.");
    }

    m_pMessageBus->EnqueueNewMsg(msg);
}

void Component::_HandleMessage(const Message &msg)
{
    if (msg.type == Message::DEFAULT)
    {
        DbgINFO("Message received!");
    }
}

void Component::HandleMessageQueue()
{
    while (!m_MessageQueue.IsEmpty())
    {
        this->_HandleMessage(*m_MessageQueue.Front());
        BroadCastToChildren(*m_MessageQueue.Front());
        m_MessageQueue.PopFront();
    }
}

bool Component::Initialize()
{
    return true;
}

bool Component::Update(const GameContext &context)
{
    return true;
}

bool Component::Shutdown()
{
    m_pMessageBus = nullptr;
    m_MessageQueue.~CBQueue();
    m_Components.~CBVector();
    return true;
}

void Component::SetActive(bool inBool)
{
    m_bIsActive = inBool;
}

void Component::AttachTo_NonSceneNode_Parent(Component* parentPtr)
{
    parentPtr->AddChild(this);
    m_pParentComponent = parentPtr;
}

void Component::AddChild(Component* childPtr)
{
    // TODO: walk the component tree to make sure there is no cycle.

    m_Components.Push_back(childPtr);
}

void Component::BroadCastToChildren(const Message &msg)
{
    for (int i = 0; i < m_Components.Size(); ++i)
    {
        m_Components.at(i)->AcceptMessage(msg);

        // TEMP: manual handle.
        m_Components.at(i)->HandleMessageQueue();
    }

}

void Component::AttachTo_SceneNode_Parent(SceneNode* parentPtr)
{
    AttachTo_NonSceneNode_Parent(parentPtr);
    DbgAssert(parentPtr == m_pParentComponent, "SceneNode pointer component attached to must be the same as parent ptr!");
    m_pParentSceneNode = parentPtr;
}

