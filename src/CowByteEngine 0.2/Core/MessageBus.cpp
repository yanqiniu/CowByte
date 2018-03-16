#include "MessageBus.h"
#include "Component.h"


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
    for (int i = 0; i < m_Subscribers.Size(); ++i)
    {
        m_Subscribers.at(i)->AcceptMessage(pMsg);
        ++numSentTo;
    }

    // Remember to pop the message after this if it was a 
    // queued message.
    return numSentTo;
}

// Call HandleMessagesQueueTree() on each subscribers.
void MessageBus::SubsHandleMessagesQueueTree()
{
    for (int i = 0; i < m_Subscribers.Size(); ++i)
    {
        m_Subscribers[i]->HandleMessagesQueueTree();
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
    for (int i = 0; i < m_Subscribers.Size(); ++i)
    {
        if (m_Subscribers.peekat(i) == newSubscbr)
        {
            return;
        }
    }

    m_Subscribers.Push_back(newSubscbr);
}


void CBMessaging::InitializeEngineBus()
{
    MessageBus::GetEngineBus() = new MessageBus();
}

void CBMessaging::PostMessage(CBRefCountPtr<Message> &pMsg, MessageBus *pMsgBus)
{
    if (pMsgBus == nullptr)
    {
        DbgWARNING("Trying to post to null message bus.");
        return;
    }

    pMsgBus->EnqueueNewMsg(pMsg);
}

// Post a message that gets broadcast to all subscribers and HANDLED right away, instead 
// of having to wait until near end of frame.
// NOTICE: this preempts all queued Messages - in fact, it doesn't even put this Message 
// into the queue. It also doesn't respect the per-engine-loop messaging processing sequence.
// Use this when you absolutely have to.
void CBMessaging::PostImediateMessage(CBRefCountPtr<Message> &pMsg, MessageBus *pMsgBus)
{
    if (pMsgBus == nullptr)
    {
        DbgWARNING("Trying to post to null message bus.");
        return;
    }

    pMsgBus->BroadcastSingle(pMsg);
}
