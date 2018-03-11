#include "MessageBus.h"
#include "Component.h"

bool MessageBus::EnqueueNewMsg(const Message &msg)
{
    return m_MessageQueue.Enqueue(msg);
}

int MessageBus::BroadCastFrontMsg()
{
    int numSentTo = 0;
    for (auto subscriber : m_Subscribers)
    {
        subscriber->_AcceptMessage(*m_MessageQueue.Front());
        ++numSentTo;
    }

    m_MessageQueue.PopFront();
    return numSentTo;
}

// For object that we want message bus not to send messages
// directly to, don't call this method on it.
void MessageBus::AddSubscriber(Component* newSubscbr)
{
    newSubscbr->SetMessageBus(this);
    m_Subscribers.Push_back(newSubscbr);
}

