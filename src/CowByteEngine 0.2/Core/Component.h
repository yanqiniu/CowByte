#ifndef _COMPONENT_H
#define _COMPONENT_H

#include "../Utils/CBQueue.h"
#include "../Utils/CBVector.h"
#include "Message.h"

class MessageBus;
class GameContext;

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

private:
    MessageBus *m_MessageBus; // TODO: multiple message bus support.
    CBQueue<Message> m_MessageQueue;
    CBVector<Component*> m_Components;
};

#endif

