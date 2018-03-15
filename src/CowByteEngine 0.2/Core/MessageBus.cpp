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
        toRet += BroadCastFrontMsg();
    }
    
    return toRet;
}



int MessageBus::BroadCastFrontMsg()
{
    int numSentTo = 0;
    for (int i = 0; i < m_Subscribers.Size(); ++i)
    {
        m_Subscribers.at(i)->AcceptMessage(*m_MessageQueue.Front());
        ++numSentTo;
    }

    m_MessageQueue.PopFront();
    return numSentTo;
}

// For object that we want message bus not to send messages
// directly to, don't call this method on it.
void MessageBus::AddSubscriber(Component* newSubscbr)
{
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


void CB::InitializeEngineBus()
{
    MessageBus::GetEngineBus() = new MessageBus();
}

void CB::PostMessage(CBRefCountPtr<Message> &pMsg, MessageBus *msgBus)
{
    if (msgBus == nullptr)
    {
        DbgWARNING("Trying to post to null message bus.");
    }

    msgBus->EnqueueNewMsg(pMsg);
}
