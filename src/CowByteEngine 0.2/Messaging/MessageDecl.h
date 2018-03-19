#ifndef _MESSAGE_DECL_H
#define _MESSAGE_DECL_H
#include "../Utils/typedefs.h"

// Used to hide constructor so no one outside can use them
#define DECLARE_MESSAGE_CONSTRUCTORS(MessageClassName)  MessageClassName() {} \
                                                        MessageClassName(const Message &toCopy) {} \
                                                        MessageClassName& operator=(const Message &rhs);

#define DECLARE_MESSAGE_CLASS(ClassName, MsgClassID)                    \
public:                                                                 \
    static MsgTypeID ClassTypeSpecifier() { return MsgClassID; }        \
    static CBRefCountPtr<Message> Create()                              \
    {                                                                   \
        ClassName *ptr = new ClassName();                               \
        ptr->m_TypeIdentifier = ClassName::ClassTypeSpecifier();        \
        return CBRefCountPtr<Message>(static_cast<Message*>(ptr));      \
    }                                                                   \
    /* Type of the instance. */                                         \
    virtual MsgTypeID GetInstType() { return m_TypeIdentifier; }        \
protected:                                                              \
    DECLARE_MESSAGE_CONSTRUCTORS(ClassName);                            \
private:                                                                \
    MsgTypeID m_TypeIdentifier;

// Extract proper message type from a CBRefCountPtr<Message>.
#define MESSAGE_FROM_PTR( msgptr, messageType ) static_cast<messageType*>(msgptr.Get())


#endif // !_MESSAGE_DECL_H
