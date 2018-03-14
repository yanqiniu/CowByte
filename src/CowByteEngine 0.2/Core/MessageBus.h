#ifndef _MESSAGE_BUS_H
#define _MESSAGE_BUS_H

#include "../Utils/CBQueue.h"
#include "../Utils/CBVector.h"
#include "Message.h"

class Component;

class MessageBus
{
public:
    MessageBus();
    ~MessageBus();
    bool EnqueueNewMsg(const Message &msg);
    int AttempBroadCastFrontMsg();
    void AddSubscriber(Component* newSubscbr);

private:
    int BroadCastFrontMsg();

    CBQueue<Message> m_MessageQueue;
    CBVector<Component*> m_Subscribers;
};

inline bool MessageBus::EnqueueNewMsg(const Message &msg)
{
    return m_MessageQueue.Enqueue(msg);
}

#endif
