#ifndef _COMPONENT_H
#define _COMPONENT_H

#include "../Utils/CBQueue.h"
#include "../Utils/CBVector.h"
#include "Message.h"

class MessageBus;
class GameContext;
class SceneNode;

class Component
{
public:
    Component();
    virtual ~Component();

    void SetMessageBus(MessageBus *mBus);
    void _AcceptMessage(const Message &msg);
    virtual void _HandleMessage(const Message &msg);

    bool Initialize();
    bool Update(const GameContext &context);
    bool Shutdown();
    void SetActive(bool inBool);

    void AttachTo_NonSceneNode_Parent(Component* parentPtr);
    void AttachTo_SceneNode_Parent(SceneNode* parentPtr);



protected:
    CBQueue<Message> m_MessageQueue;
    CBVector<Component*> m_Components;
    Component *m_pParentComponent;
    SceneNode *m_pParentSceneNode; // Scene node this component is attached to. nullptr if none.

private:
    void AddChild(Component* childPtr);

    MessageBus *m_pMessageBus; // TODO: multiple message bus support.

    bool m_bIsActive;
};

#endif

