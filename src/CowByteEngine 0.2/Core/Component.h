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
    enum CompType
    {
        // TODO: maybe I can add like a macro to 
        // register a type automatically?
        DEFAULT,
        SYSTEM,
        MESHINSTANCE,
        NUM_TYPES
    };

    Component();
    virtual ~Component();

    void SetMessageBus(MessageBus *mBus);
    void AcceptMessage(const Message &msg);
    void PostMessage(const Message &msg);

    bool Initialize();
    bool Update(const GameContext &context);
    bool Shutdown();
    void SetActive(bool inBool);

    void AttachTo_NonSceneNode_Parent(Component* parentPtr);
    void AttachTo_SceneNode_Parent(SceneNode* parentPtr);

    SceneNode *GetParentSceneNode() const;
    virtual void HandleMessageQueue();


protected:
    virtual void _HandleMessage(const Message &msg);
    void BroadCastToChildren(const Message &msg);

    CBQueue<Message> m_MessageQueue;
    CBVector<Component*> m_Components;
    Component *m_pParentComponent;
    SceneNode *m_pParentSceneNode; // Scene node this component is attached to. nullptr if none.
    CompType m_CompType;

private:
    void AddChild(Component* childPtr);

    MessageBus *m_pMessageBus; // TODO: multiple message bus support.

    bool m_bIsActive;
};

inline SceneNode *Component::GetParentSceneNode() const
{
    return m_pParentSceneNode;
}


#endif

