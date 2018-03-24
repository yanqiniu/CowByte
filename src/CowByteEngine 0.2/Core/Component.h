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

class Component
{
public:
    CBMEM_OVERLOAD_NEW_DELETE(Component)
    Component();
    virtual ~Component();

    void AcceptMessage(CBRefCountPtr<Message> &pMsg);

    virtual bool Initialize();
    virtual bool Shutdown();
    bool UpdateTree(const GameContext &context); // A "larger" update function that calls Update on this and offspring.
    void SetActive(bool inBool);
    bool IsActiveSelf();
    size_t GetNumOffsprings() const { return m_nOffsprings; };


    void AttachTo_NonSceneNode_Parent(Component* parentPtr);
    void AttachTo_SceneNode_Parent(SceneNode* parentPtr);
    bool HasSceneNodeParent();
    SceneNode *GetParentSceneNode() const;
    void HandleMessagesQueueTree();



protected:
    virtual bool Update(const GameContext &context) = 0;
    virtual void _HandleMessage(CBRefCountPtr<Message> &pMsg) = 0;
    void HandleMessageQueue();
    static void IncreNumOffsprings(Component* comp, size_t amount);

    CBQueue<CBRefCountPtr<Message>> m_MessageQueue;
    CBVector<Component*> m_Components;
    Component *m_pParentComponent;
    bool m_bParentIsSceneNode;

private:
    bool AddChild(Component* childPtr);
    size_t m_nOffsprings; // Total number of nodes below this one.
    bool m_bIsActive;
};



#endif

