#include "MessageBus.h"
#include "../Core/Component.h"


MessageBus* MessageBus::s_pEngineBus;

MessageBus::MessageBus() :
    m_MessageQueue(),
    m_Subscribers(8)
{

}

MessageBus::~MessageBus()
{

}

// Broad cast all messages to all subscribers.
int MessageBus::Broadcast()
{
    int toRet = 0;
    while (!m_MessageQueue.IsEmpty())
    {
        toRet += BroadcastSingle(*m_MessageQueue.Front());
        m_MessageQueue.PopFront();
    }
    
    return toRet;
}


// Broadcast a single message to all subscribers.
int MessageBus::BroadcastSingle(CBRefCountPtr<Message> &pMsg)
{
    int numSentTo = 0;
    for (auto& sub : m_Subscribers)
    {
        sub->AcceptMessage(pMsg);
        ++numSentTo;
    }

    // Remember to pop the message after this if it was a 
    // queued message.
    return numSentTo;
}

// Call HandleMessagesQueueTree() on each subscribers.
void MessageBus::SubsHandleMessagesQueueTree()
{
    for (auto& sub : m_Subscribers)
    {
        sub->HandleMessagesQueueTree();
    }
}

// For object that we want message bus not to send messages
// directly to, don't call this method on it.
void MessageBus::AddSubscriber(Component* newSubscbr)
{
    if (newSubscbr == nullptr)
    {
        DbgWARNING("Trying to add null subscriber to message bus.");
        return;
    }

    // Prevent double subscription.
    for (const auto& sub : m_Subscribers)
    {
        if (sub == newSubscbr)
        {
            return;
        }
    }

    m_Subscribers.Push_back(newSubscbr);
}

