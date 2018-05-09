#ifndef _CBMESSAGING_H
#define _CBMESSAGING_H

#include "Message.h"
#include "MessageBus.h"

namespace  CBMessaging
{
    void InitializeEngineBus();
    void PostQueuedMessage(CBRefCountPtr<Message> &pMsg, MessageBus *pMsgBus);
    void PostImediateMessage(CBRefCountPtr<Message> &pMsg, MessageBus *pMsgBus);
    // TODO: Post immediate message to a specific component to avoid walking the whole tree.
}


#endif //!_CBMESSAGING_H
