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
    bool EnqueueNewMsg(CBRefCountPtr<Message> pMsg);
    void AddSubscriber(Component* newSubscbr);
    int Broadcast();

    static MessageBus *& GetEngineBus();

private:
    static MessageBus* s_pEngineBus;
    int BroadCastFrontMsg();

    CBQueue<CBRefCountPtr<Message>> m_MessageQueue;
    CBVector<Component*> m_Subscribers;
};

inline bool MessageBus::EnqueueNewMsg(CBRefCountPtr<Message> pMsg)
{
    return m_MessageQueue.Enqueue(pMsg);
}

inline MessageBus *& MessageBus::GetEngineBus()
{
    return s_pEngineBus;
}

namespace  CB
{
    void InitializeEngineBus();

}

#endif
