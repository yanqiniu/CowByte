#ifndef _CBQUEUE_H
#define _CBQUEUE_H

#include "typedefs.h"

template<typename DataT>
class CBQueue
{
public:
    CBQueue() :
        m_Size(0),
        m_Head(nullptr),
        m_Tail(nullptr)
    {};

    ~CBQueue()
    {
        while (!IsEmpty())
        {
            PopFront();
        }
    }

    struct CBQueueNode
    {
        CBQueueNode() : _next(nullptr) {}

        DataT _data;
        CBQueueNode *_next;
    };

    inline bool IsEmpty()
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

