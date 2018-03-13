#ifndef _CBQUEUE_H
#define _CBQUEUE_H

#include "typedefs.h"
#include "../Memory/CBMemArena.h"

template<typename DataT>
class CBQueue
{
public:
    CBQueue() :
        m_Size(0),
        m_Head(nullptr),
        m_Tail(nullptr)
    {};

    CBQueue(const CBQueue &toCopy) :
        m_Size(0),
        m_Head(nullptr),
        m_Tail(nullptr)
    {
        for (CBQueueNode *node = toCopy.Front(); node != toCopy.Back(); node = node->_next)
        {
            Enqueue(node->_data);
        }
    };


    ~CBQueue()
    {
        while (!IsEmpty())
        {
            PopFront();
        }
    }

    // Need to explicitly destruct the CBQueue before
    // calling this, if the queue is not empty. Since 
    // there's no way of knowing whether the queue has 
    // been initialized. You can find a similar and more 
    // comprehensive reasoning about this in CBVector's
    // assignment operator overload.
    CBQueue & operator=(const CBQueue &rhs)
    {
        // This should be an empty object now.

        m_Size = 0;
        m_Head = nullptr;
        m_Tail = nullptr;

        for (CBQueueNode *node = rhs.Front(); node != rhs.Back(); node = node->_next)
        {
            Enqueue(node->_data);
        }

        return *this;

    }

    struct CBQueueNode
    {
        CBQueueNode() : _next(nullptr) {}

        DataT _data;
        CBQueueNode *_next;
    };

    inline bool IsEmpty() const
    {
        return m_Size == 0;
    }

    bool Enqueue(const DataT& dataToAdd)
    {
        CBQueueNode *node = new CBQueueNode();
        if (!node)
            return false;
        node->_data = dataToAdd;

        if (IsEmpty())
        {
            m_Head = node;
            m_Tail = node;
        }
        else
        {
            m_Tail->_next = node;
            m_Tail = node;
        }

        ++m_Size;
        return true;
    }

    inline DataT* Front()
    {
        return &m_Head->_data;
    }

    inline DataT* Back()
    {
        return &m_Tail->_data;
    }

    void PopFront()
    {
        if (m_Size == 0)
        {
            return;
        }
        else if (m_Size == 1)
        {
            delete m_Head;
            m_Head = nullptr;
            m_Tail = nullptr;
            --m_Size;
        }
        else
        {
            CBQueueNode *temp = m_Head;
            m_Head = m_Head->_next;
            delete temp;
            --m_Size;
        }
    }


private:
    CBQueueNode * m_Head;
    CBQueueNode *m_Tail;
    UINT32 m_Size; // Num of elements currently in the queue;
};

#endif // _CBQUEUE_H

