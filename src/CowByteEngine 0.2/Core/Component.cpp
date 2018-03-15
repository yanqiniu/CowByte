#include "Component.h"
#include "MessageBus.h"
#include "../Utils/CBQueue.h"
#include "../Utils/CBDebug.h"
#include "../SceneGraph/SceneNode.h"

Component::Component() :
    m_MessageQueue(),
    m_Components(4),
    m_bIsActive(true),
    m_pParentComponent(nullptr),
    m_pParentSceneNode(nullptr)
{

}

Component::~Component()
{
    Shutdown();
}

void Component::AcceptMessage(CBRefCountPtr<Message> &pMsg)
{
    m_MessageQueue.Enqueue(pMsg);

    // And then broadcast to all children.
    for (UINT32 i = 0; i < m_Components.Size(); ++i)
    {
        m_Components.at(i)->AcceptMessage(pMsg);
    }
}

void Component::PostMessage(CBRefCountPtr<Message> &pMsg, MessageBus *msgBus)
{
    if (msgBus == nullptr)
    {
        DbgWARNING("Trying to post to null message bus.");
    }

    msgBus->EnqueueNewMsg(pMsg);

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
    m_MessageQueue.~CBQueue();
    m_Components.~CBVector();
    m_pParentComponent = nullptr;
    m_pParentSceneNode = nullptr;

    return true;
}

bool Component::UpdateTree(const GameContext &context)
{
    bool toRet = true;
    // Update me and children:
    toRet &= Update(context);
    for (int i = 0; i < m_Components.Size(); ++i)
    {
        toRet &= m_Components.at(i)->UpdateTree(context);
    }

    return toRet;
}

void Component::SetActive(bool inBool)
{
    m_bIsActive = inBool;
}

// Doesn't care if a parent is disabled.
bool Component::IsActiveSelf()
{
    return m_bIsActive;
}

void Component::AttachTo_NonSceneNode_Parent(Component* parentPtr)
{
    parentPtr->AddChild(this);
    m_pParentComponent = parentPtr;
}

void Component::AttachTo_SceneNode_Parent(SceneNode* parentPtr)
{
    AttachTo_NonSceneNode_Parent(parentPtr);
    DbgAssert(parentPtr == m_pParentComponent, "SceneNode pointer component attached to must be the same as parent ptr!");
    m_pParentSceneNode = parentPtr;
}

void Component::AddChild(Component* childPtr)
{
    // TODO: walk the component tree to make sure there is no cycle.

    m_Components.Push_back(childPtr);
}

// All children should have received their messages when this is called.
void Component::HandleMessageQueue()
{
    while (!m_MessageQueue.IsEmpty())
    {
        _HandleMessage(*m_MessageQueue.Front());
        m_MessageQueue.PopFront();
    }
}

void Component::HandleMessagesQueueTree()
{
    HandleMessageQueue();
    for (int i = 0; i < m_Components.Size(); ++i)
    {
        m_Components.at(i)->HandleMessagesQueueTree();
    }
}


