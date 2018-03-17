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
    m_bParentIsSceneNode(false)
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
    m_bParentIsSceneNode = true;
}

bool Component::HasSceneNodeParent()
{
    return m_bParentIsSceneNode;
}

SceneNode *Component::GetParentSceneNode() const
{
    if (m_bParentIsSceneNode)
        return dynamic_cast<SceneNode*> (m_pParentComponent);
    else
        return nullptr;
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


