#include "Message.h"


// Factory method.
CBRefCountPtr<Message> Message::Create(MessageType type)
{
#define CREATE_FROM_TYPE(typeOfMsg, classnameOfMsg) if (type == typeOfMsg) \
{\
            classnameOfMsg *ptr = new classnameOfMsg();\
            ptr->Initialize();\
            return CBRefCountPtr<Message>(static_cast<Message*>(ptr));\
}

    // Specify a new type/class pair here.
    CREATE_FROM_TYPE(MessageType::MsgType_RegisterDrawbleMeshInstance, Msg_RegisterDrawbleMeshInst);
    CREATE_FROM_TYPE(MessageType::MsgType_SetMainCamera, Msg_SetMainCamera);

    return CBRefCountPtr<Message>(nullptr);
#undef CREATE_FROM_TYPE
}

