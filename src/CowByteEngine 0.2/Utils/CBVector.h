#ifndef _CBVECTOR_H
#define _CBVECTOR_H

#include <cstring>
#include "../Memory/CBMemArena.h"

// Unlike CBQueue who implement a block based linked list, 
// CBVector implement a contiguous array in a single block.
template <class T>
class CBVector
{
public:
    CBVector();
    CBVector(const CBVector &toCopy);
    CBVector(size_t size); // Initialize the container to a size.
    ~CBVector();
    CBVector<T> & operator=(const CBVector<T> &rhs);

    size_t Size() const { return m_Size; }
    size_t Capacity() const { return m_Capacity; }
    void   Resize(size_t newCapacity); /* Change capacity of container. */
    bool   IsEmpty() const { return m_Size == 0; }

    const T& peekat(size_t index) const { return m_Data[index]; } // const version of at().
    T& at(size_t index) { return m_Data[index]; }
    T& operator[](size_t index) { return m_Data[index]; }

    // This is always a copy construct.
    T*   Push_back(const T &toPush);
    bool Pop_back();
    T*   Insert(size_t index, const T &value);
    bool Erase(size_t index);
    void Clear();

    // TODO: what is wrong with these that range-based for loop don't work?
    T* begin() const
    {
        return &m_Data[0];
    }
    T* end() const
    {
        return &m_Data[m_Size - 1];
    }

private:
    // Make array larger.
    void Grow_capacity();
    bool IsValidIndex(size_t index);

    T* m_Data;
    size_t m_Capacity;
    size_t m_Size;
};


template <class T>
CBVector<T>::CBVector() :
    m_Size(0),
    m_Capacity(0),
    m_Data(nullptr)
{
}

// Custom copy constructor that doesn't just do a shallow copy.
template <class T>
CBVector<T>::CBVector(const CBVector &toCopy) :
    m_Size(0),
    m_Capacity(0),
    m_Data(nullptr)
{
    Resize(toCopy.m_Capacity);
    memcpy(m_Data, toCopy.m_Data, m_Size * sizeof(T));
    m_Size = toCopy.m_Size;
}

template <class T>
CBVector<T>::CBVector(size_t size) :
    CBVector()
{
    Resize(size);
}

template <class T>
CBVector<T>::~CBVector()
{
    Clear();
    if (m_Data != nullptr)
    {
        CBMemArena::Get().Free(m_Data, m_Capacity * sizeof(T));
        m_Data = nullptr;
    }
}

template <class T>
CBVector<T> & CBVector<T>::operator=(const CBVector<T> &rhs)
{
    /*
    Ok, here's the problem.

    To assign a CBVector to a new value, we'd want to destruct stuff already
    in it, if there happens to be any.

    Since assignment operator is used to initialize objects, it means that 
    when we run into this func, the class can be either uninitialized or
    filled with stuff that needs to be destructed.

    And there isn't really a reliable way of knowing which case it is. 
    And if it happens to be uninitialized, we are gonna get an error 
    trying to Clear() it.

    So we can only assume that this is an empty/uninitialized vector, 
    and explicitly clean it up before the assignment operator is called.
    */

    // Reset counts.
    m_Size = 0;
    m_Capacity = 0;

    // Copy data
    Resize(rhs.m_Capacity);
    memcpy(m_Data, rhs.m_Data, rhs.m_Size * sizeof(T));
    m_Size = rhs.m_Size;

    return *this;
}

template <class T>
void CBVector<T>::Clear()
{
    // Please note that this does not set capacity to 0.
    // The idea is that when you clear an array, it's 
    // possible you are going to put new stuff in.

    while(!IsEmpty())
        Pop_back();
}

template <class T>
bool CBVector<T>::IsValidIndex(size_t index)
{
    return index >= 0 && index < m_Size;
}
// Insert a copy. And return ptr to the inserted copy.
// Notice that insert in the middle is very costly as it
// needs to destruct elements being shifted when shifting
// them.
template <class T>
T* CBVector<T>::Insert(size_t index, const T &value)
{
    if (index < 0 || index > m_Size) // can insert at mSize (push_back)
        return nullptr;

    if (m_Size + 1 > m_Capacity)
        Grow_capacity();

    // Shift. Make some space.
    if(m_Size > 0)
        for (size_t i = m_Size - 1; i >= index; --i)
        {
            m_Data[i + 1] = m_Data[i];
            m_Data[i].~T();
            if(i == 0) break; // underflow guard.
        }

    m_Data[index] = value;
    ++m_Size;

    return &m_Data[index];
}

template <class T>
bool CBVector<T>::Erase(size_t index)
{
    if (!IsValidIndex(index))
        return false;

    // shift
    for (size_t i = index + 1; i < m_Size; ++i)
    {
        m_Data[i - 1] = m_Data[i];
    }

    m_Data[m_Size - 1].~T();
    memset(&m_Data[m_Size - 1], 0, sizeof(T));
    --m_Size;

    return true;
}

template <class T>
bool CBVector<T>::Pop_back()
{
    return Erase(m_Size - 1);
}

template <class T>
void CBVector<T>::Resize(size_t newCapacity)
{
    if (newCapacity == m_Capacity) // "wtf?"
        return;
    else if (newCapacity < m_Size) // shrink
    {
        // No need to shrink the actual array,
        // since that introduces performance 
        // overhead. Just destroy elements out 
        // of bound and update bookkeepings.
        for (size_t i = newCapacity; i < m_Size; ++i)
        {
            m_Data[i].~T();
            memset(&m_Data[i], 0, sizeof(T));
        }

        m_Capacity = newCapacity;
        m_Size = newCapacity;
    }
    else // grow
    {
        // Create new array
        T *newData = (T*)CBMemArena::Get().Allocate(newCapacity * sizeof(T));

        if (!IsEmpty())
        {
            memcpy(newData, m_Data, m_Size * sizeof(T));
            CBMemArena::Get().Free(m_Data, m_Capacity * sizeof(T));
            m_Data = nullptr;
        }
        m_Capacity = newCapacity;

        m_Data = newData;
    }
}

template <class T>
T* CBVector<T>::Push_back(const T &toPush)
{
    return Insert(m_Size, toPush);
}


template <class T>
void CBVector<T>::Grow_capacity()
{
    if (m_Capacity == 0)
    {
        Resize(1);
    }
    else
        Resize(m_Capacity * 2);
}


#endif
