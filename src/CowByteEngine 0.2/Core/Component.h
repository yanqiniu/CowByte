#ifndef _COMPONENT_H
#define _COMPONENT_H

#include "../Utils/CBQueue.h"
#include "../Utils/CBVector.h"
#include "../Utils/CBRefCountPtr.h"
#include "../Messaging/CBMessaging.h"
#include "../Memory/CBMemory.h"

class MessageBus;
class GameContext;
class SceneNode;

// Overload pure virtual function with default content.
#define DECLARE_COMPONENT_DEFAULT(ClassName)                                \
virtual bool Update(const GameContext &context) override { return true; };   \
virtual void _HandleMessage(CBRefCountPtr<Message> &pMsg) override {};


class Component
{
public:
    CBMEM_OVERLOAD_NEW_DELETE(Component)
    Component();
    virtual ~Component();

    // Basic:
    virtual bool Initialize();
    virtual bool Shutdown();
    bool   UpdateTree(const GameContext &context); // A "larger" update function that calls Update on this and offspring.
    void   SetActive(bool inBool);
    bool   IsActiveSelf() const;

    // Hierarchy and messaging related:
    size_t     GetNumOffsprings() const { return m_nOffsprings; };
    void       AttachTo_NonSceneNode_Parent(Component* parentPtr);
    void       AttachTo_SceneNode_Parent(SceneNode* parentPtr);
    bool       HasSceneNodeParent();
    SceneNode *GetParentSceneNode() const;
    void       AcceptMessage(CBRefCountPtr<Message> &pMsg);
    void       HandleMessagesQueueTree();

protected:
    virtual bool Update(const GameContext &context) = 0;
    virtual void _HandleMessage(CBRefCountPtr<Message> &pMsg) = 0;
    void         HandleMessageQueue();

    CBQueue<CBRefCountPtr<Message>> m_MessageQueue;
    CBVector<Component*> m_ChildrenComps;
    Component *m_pParentComp;

private:
    bool AddChild(Component* childPtr);
    void IncreNumOffsprings(size_t amount);

    size_t m_nOffsprings; // Total number of nodes below this one.
    bool m_bParentIsSceneNode;
    bool m_bIsActive;
};



#endif

