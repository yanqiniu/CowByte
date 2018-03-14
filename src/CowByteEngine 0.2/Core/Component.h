#ifndef _COMPONENT_H
#define _COMPONENT_H

#include "../Utils/CBQueue.h"
#include "../Utils/CBVector.h"
#include "../Utils/CBRefCountPtr.h"
#include "Message.h"

class MessageBus;
class GameContext;
class SceneNode;

class Component
{
public:
    Component();
    virtual ~Component();

    void AcceptMessage(CBRefCountPtr<Message> pMsg);
    void PostMessage(CBRefCountPtr<Message> pMsg, MessageBus *msgBus);

    bool Initialize();
    bool Update(const GameContext &context);
    bool Shutdown();
    void SetActive(bool inBool);
    bool IsActiveSelf();

    void AttachTo_NonSceneNode_Parent(Component* parentPtr);
    void AttachTo_SceneNode_Parent(SceneNode* parentPtr);

    SceneNode *GetParentSceneNode() const;
    virtual void HandleMessageQueue();


protected:
    virtual void _HandleMessage(CBRefCountPtr<Message> pMsg);
    void BroadcastToChildren(CBRefCountPtr<Message> pMsg);

    CBQueue<CBRefCountPtr<Message>> m_MessageQueue;
    CBVector<Component*> m_Components;
    Component *m_pParentComponent;
    SceneNode *m_pParentSceneNode; // Scene node this component is attached to. nullptr if none.

private:
    void AddChild(Component* childPtr);
    bool m_bIsActive;
};

inline SceneNode *Component::GetParentSceneNode() const
{
    return m_pParentSceneNode;
}


#endif

