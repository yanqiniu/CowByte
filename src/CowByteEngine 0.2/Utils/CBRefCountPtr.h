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
    CBRefCountPtr();
    explicit CBRefCountPtr( DataT* ptr);
    CBRefCountPtr(const CBRefCountPtr& toCopy);
    ~CBRefCountPtr();
    CBRefCountPtr<DataT> & operator=(const CBRefCountPtr<DataT>& other);


    DataT& operator*();
    DataT* operator->();
    DataT* Get() const;
    const DataT* Peek() const;

private:

    CBRefCountPtr_ControlBlock *m_pControlBlock;
    DataT *m_pData; // Actual pointer to the object.
};

template <typename DataT>
CBRefCountPtr<DataT>::CBRefCountPtr() :
    m_pControlBlock(nullptr),
    m_pData(nullptr)
{

}

template <typename DataT>
CBRefCountPtr<DataT>::CBRefCountPtr( DataT *ptr)
{
    m_pData = ptr;
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

// Shallow assignment operator that does not check if resources has to be 
// released. Because there's no way of knowing whether the object has been
// initialized. So destruct the object before assignment explicitly if you 
// know it's not empty.
template <typename DataT>
CBRefCountPtr<DataT> & CBRefCountPtr<DataT>::operator=(const CBRefCountPtr<DataT>& other)
{
    m_pData = other.m_pData;
    m_pControlBlock = other.m_pControlBlock;
    ++(m_pControlBlock->RefCount);

    return *this;
}


template <typename DataT>
CBRefCountPtr<DataT>::~CBRefCountPtr<DataT>()
{
    --(m_pControlBlock->RefCount);

    if (m_pControlBlock->RefCount <= 0) // technically should never go below zero
    {
        delete m_pData;
        delete m_pControlBlock;
    }
}

template <typename DataT>
DataT& CBRefCountPtr<DataT>::operator*()
{
    return *m_pData;
}

template <typename DataT>
DataT* CBRefCountPtr<DataT>::operator->()
{
    return m_pData;
}


template <typename DataT>
DataT* CBRefCountPtr<DataT>::Get() const
{
    return m_pData;
}


template <typename DataT>
const DataT* CBRefCountPtr<DataT>::Peek() const
{
    return m_pData;
}




#endif
