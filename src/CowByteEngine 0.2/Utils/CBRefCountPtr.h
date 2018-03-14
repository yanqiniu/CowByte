#ifndef _CBREFCOUNT_PTR_H
#define _CBREFCOUNT_PTR_H

#include "typedefs.h"

// ControlBlock for RefCount Ptr.
struct CBRefCountPtr_ControlBlock
{
    INT32 RefCount;
};

// CBE reference counting pointer.
template <typename DataT>
class CBRefCountPtr
{
public:
    explicit CBRefCountPtr(T* ptr);
    CBRefCountPtr(const CBRefCountPtr& toCopy);
    ~CBRefCountPtr();

    T& operator*();
    T* operator->();

private:
    // Disallow assignment operator to avoid complicating it...
    CBRefCountPtr & operator=(const CBRefCountPtr& other);

    CBRefCountPtr_ControlBlock *m_pControlBlock;
    DataT *m_pData; // Actual pointer to the object.
};

template <typename DataT>
CBRefCountPtr<DataT>::CBRefCountPtr(T* ptr)
{
    m_pControlBlock = new CBRefCountPtr_ControlBlock();
    m_pControlBlock->RefCount = 1;
}

template <typename DataT>
CBRefCountPtr<DataT>::CBRefCountPtr(const CBRefCountPtr& toCopy)
{
    m_pData = toCopy.m_pData;
    m_pControlBlock = toCopy.m_pControlBlock;

    ++(m_pControlBlock->RefCount);
}

template <typename DataT>
CBRefCountPtr<DataT>::~CBRefCountPtr()
{
    --(m_pControlBlock->RefCount);

    if (m_pControlBlock->RefCount <= 0) // technically should never go below zero
    {
        delete m_pData;
        delete m_pControlBlock;
    }
}

template <typename DataT>
T& CBRefCountPtr<DataT>::operator*()
{
    return *m_pData;
}

template <typename DataT>
T* CBRefCountPtr<DataT>::operator->()
{
    return m_pData;
}

template <typename DataT>
CBRefCountPtr & CBRefCountPtr<DataT>::operator=(const CBRefCountPtr& other)
{

}

#endif
