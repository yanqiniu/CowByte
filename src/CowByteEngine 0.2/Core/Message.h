#ifndef _MESSAGE_H
#define _MESSAGE_H

class MeshInstance;

struct Message
{
    enum MessageType
    {
        DEFAULT,
        RegisterDrawbleMeshInstance
    };
    Message() : type(DEFAULT) {}

    MessageType type;
};

struct Msg_RegisterDrawbleMeshInst : public Message
{
    explicit Msg_RegisterDrawbleMeshInst(MeshInstance *meshInstPtr) :
        m_MeshInstPtr(meshInstPtr)
    {
        type = RegisterDrawbleMeshInstance;
    }
    MeshInstance *m_MeshInstPtr;
};

#endif // !_MESSAGE_H
