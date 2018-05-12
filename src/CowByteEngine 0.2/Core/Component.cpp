#include "Component.h"
#include "../Utils/CBQueue.h"
#include "../Utils/CBDebug.h"
#include "../SceneGraph/SceneNode.h"

Component::Component() :
    m_MessageQueue(),
    m_ChildrenComps(4),
    m_bIsActive(true),
    m_pParentComp(nullptr),
    m_bParentIsSceneNode(false),
    m_nOffsprings(0)
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
    for (UINT32 i = 0; i < m_ChildrenComps.Size(); ++i)
    {
        m_ChildrenComps.at(i)->AcceptMessage(pMsg);
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
    m_ChildrenComps.~CBVector();
    m_pParentComp = nullptr;

    return true;
}

bool Component::UpdateTree(const GameContext &context)
{
    bool toRet = true;
    // Update me and children:
    toRet &= Update(context);
    for (auto& comp : m_ChildrenComps)
    {
        toRet &= comp->UpdateTree(context);
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
    // Erasing component reference in current parent, if there is one.
    if (m_pParentComp != nullptr && m_pParentComp != parentPtr)
    {
        for (size_t i = 0; i < m_pParentComp->m_ChildrenComps.Size(); ++i)
        {
            if (m_pParentComp->m_ChildrenComps.peekat(i) == this)
            {
                m_pParentComp->m_ChildrenComps.Erase(i);
            }
        }
    }

    parentPtr->AddChild(this);
    m_pParentComp = parentPtr;
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
        return dynamic_cast<SceneNode*> (m_pParentComp);
    else
        return nullptr;
}


bool Component::AddChild(Component* childPtr)
{
    // Check if the pointer already exists.
    for (const auto& comp : m_ChildrenComps)
    {
        if (comp == childPtr)
            return false;
    }

    m_ChildrenComps.Push_back(childPtr);
    IncreNumOffsprings(1 + childPtr->m_nOffsprings);
    return true;
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

void Component::IncreNumOffsprings(size_t amount)
{
    Component *comp = this;
    while (comp != nullptr)
    {
        comp->m_nOffsprings += amount;
        comp = comp->m_pParentComp;
    }
}

void Component::HandleMessagesQueueTree()
{
    HandleMessageQueue();
    for (auto& comp : m_ChildrenComps)
    {
        comp->HandleMessagesQueueTree();
    }
}


