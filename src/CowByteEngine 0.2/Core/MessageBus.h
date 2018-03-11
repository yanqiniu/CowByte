#ifndef _MESSAGE_BUS_H
#define _MESSAGE_BUS_H

#include "../Utils/CBQueue.h"
#include "../Utils/CBVector.h"

class Message;
class Component;

class MessageBus
{
public:
    MessageBus();
    ~MessageBus();
    bool EnqueueNewMsg(const Message &msg);
    int BroadCastFrontMsg();
    void AddSubscriber(Component* newSubscbr);

private:
    CBQueue<Message> m_MessageQueue;
    CBVector<Component*> m_Subscribers;
};

#endif
