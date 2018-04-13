#include "CBMessaging.h"

void CBMessaging::InitializeEngineBus()
{
    MessageBus::GetEngineBus() = new MessageBus();
}

void CBMessaging::PostQueuedMessage(CBRefCountPtr<Message> &pMsg, MessageBus *pMsgBus)
{
    if (pMsgBus == nullptr)
    {
        DbgWARNING("Trying to post to null message bus.");
        return;
    }

    pMsgBus->EnqueueNewMsg(pMsg);
}

void CBMessaging::PostImediateMessage(CBRefCountPtr<Message> &pMsg, MessageBus *pMsgBus)
{
    if (pMsgBus == nullptr)
    {
        DbgWARNING("Trying to post to null message bus.");
        return;
    }

    pMsgBus->BroadcastSingle(pMsg);
}
