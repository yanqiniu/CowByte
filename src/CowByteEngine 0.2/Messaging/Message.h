#ifndef _MESSAGE_H
#define _MESSAGE_H
#include "../Utils/CBRefCountPtr.h"
#include "../Utils/typedefs.h"
#include "MessageDecl.h"
#include "../Memory/CBMemory.h"

class MeshInstance;
class Camera;
/*

Step-by-Step: How do we create a new Message type?

1. Declare the class below, add custom data members if needed.
2. Use DECLARE_MESSAGE_CLASS(ClassName, TypeSpecifier) at beginning of class.
3. !!!TypeSpecifier bust be different for each message type!!

*/


typedef INT32 MsgTypeID;




class Message
{
public:
    CBMEM_OVERLOAD_NEW_DELETE(Message);
    static MsgTypeID ClassTypeSpecifier() { return 0; }

    // Returns null pointer. Do not call.
    static CBRefCountPtr<Message> Create() { return CBRefCountPtr<Message>(nullptr); }
    /* Type of the instance. */
    virtual MsgTypeID GetInstType() = 0;
protected:
    DECLARE_MESSAGE_CONSTRUCTORS(Message);
private:
    MsgTypeID m_TypeIdentifier;
};


class Msg_RegisterDrawbleMeshInst : public Message
{
    DECLARE_MESSAGE_CLASS(Msg_RegisterDrawbleMeshInst, 1);
public:
    MeshInstance *m_MeshInstPtr;
};


class Msg_SetMainCamera : public Message
{
    DECLARE_MESSAGE_CLASS(Msg_SetMainCamera, 2);
public:
    Camera *m_pCamera;
};

#endif // !_MESSAGE_H
