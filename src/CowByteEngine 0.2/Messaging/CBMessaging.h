#ifndef _CBMESSAGING_H
#define _CBMESSAGING_H

#include "Message.h"
#include "MessageBus.h"

namespace  CBMessaging
{
    void InitializeEngineBus();
    void PostQueuedMessage(CBRefCountPtr<Message> &pMsg, MessageBus *pMsgBus);
    void PostImediateMessage(CBRefCountPtr<Message> &pMsg, MessageBus *pMsgBus);
}


#endif //!_CBMESSAGING_H
