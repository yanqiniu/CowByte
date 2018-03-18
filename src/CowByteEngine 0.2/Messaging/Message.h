#ifndef _MESSAGE_H
#define _MESSAGE_H
#include "../Utils/CBRefCountPtr.h"
class MeshInstance;

/*

Step-by-Step: How do we create a new Message type?

1. Declare a new MessageType in MESSAGE_TYPES macro.
2. Define the class in this file.
3. Forward declare the class before Message.
4. Specify the type / class pair in Message::Create()

This may be simplified in the future.

*/

// register a new type here.
#define MESSAGE_TYPES \
msgtype(Default), \
msgtype(RegisterDrawbleMeshInstance)

#define msgtype(x) MsgType_##x
typedef enum {MESSAGE_TYPES} MessageType; // Now we have the types
// used to hide constructor so no one outside can use them
#define DECLARE_MESSAGE_CONSTRUCTORS(MessageClassName)  MessageClassName() {} \
                                                        MessageClassName(const Message &toCopy) {} \
                                                        MessageClassName& operator=(const Message &rhs);

// Declare different types of messages here.
struct Msg_RegisterDrawbleMeshInst;

struct Message
{
    virtual void Initialize()
    {
        type = MessageType::MsgType_Default;
    }
    static CBRefCountPtr<Message> Create(MessageType type);


    MessageType type;

protected:
    DECLARE_MESSAGE_CONSTRUCTORS(Message);

};

struct Msg_RegisterDrawbleMeshInst : public Message
{
    friend Message;
    virtual void Initialize()
    {
        type = MessageType::MsgType_RegisterDrawbleMeshInstance;
    }

    // Data members
    MeshInstance *m_MeshInstPtr;
protected:
    DECLARE_MESSAGE_CONSTRUCTORS(Msg_RegisterDrawbleMeshInst);
};

#endif // !_MESSAGE_H
