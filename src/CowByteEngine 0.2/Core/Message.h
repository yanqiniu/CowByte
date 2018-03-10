#ifndef _MESSAGE_H
#define _MESSAGE_H

struct Message
{
    enum MessageType
    {
        DEFAULT
    };

    MessageType type;
};

#endif // !_MESSAGE_H
