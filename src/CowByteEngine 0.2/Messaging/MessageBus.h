#ifndef _MESSAGE_BUS_H
#define _MESSAGE_BUS_H

#include "../Utils/CBQueue.h"
#include "../Utils/CBVector.h"
#include "../Utils/CBRefCountPtr.h"
#include "Message.h"

class Component;

class MessageBus
{
public:

    MessageBus();
    ~MessageBus();
    bool EnqueueNewMsg(CBRefCountPtr<Message> &pMsg);
    void AddSubscriber(Component* newSubscbr);
    int  Broadcast();
    int  BroadcastSingle(CBRefCountPtr<Message> &pMsg);
    void SubsHandleMessagesQueueTree();

    static MessageBus *& GetEngineBus();

private:
    static MessageBus* s_pEngineBus;

    CBQueue<CBRefCountPtr<Message>> m_MessageQueue;
    CBVector<Component*> m_Subscribers;
};

inline bool MessageBus::EnqueueNewMsg(CBRefCountPtr<Message> &pMsg)
{
    return m_MessageQueue.Enqueue(pMsg);
}

inline MessageBus *& MessageBus::GetEngineBus()
{
    return s_pEngineBus;
}


#endif
