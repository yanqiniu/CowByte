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
    CBVector(size_t size); // Initialize the container to a size.
    ~CBVector();

    size_t Size() const { return m_Size; }
    size_t Capacity() const { return m_Capacity; }
    void Resize(size_t newCapacity); /* Change capacity of container. */
    bool IsEmpty() const { return m_Size == 0; }

    T& at(size_t index) { return m_Data[index]; }
    T& operator[](size_t index) { return m_Data[index]; }
    // This is always a copy construct.
    T* Push_back(const T &toPush);
    bool Pop_back();

    T* Insert(size_t index, const T &value);
    bool Erase(size_t index);

    void Clear();

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

template <class T>
CBVector<T>::CBVector(size_t size) :
    CBVector()
{
    Resize(size);
}

template <class T>
CBVector<T>::~CBVector()
{
    if (m_Data != nullptr)
    {
        CBMemArena::Get().Free(m_Data, m_Capacity * sizeof(T));
        m_Data = nullptr;
    }
}

template <class T>
void CBVector<T>::Clear()
{
    // Please note that this does not set capacity to 0.
    // The idea is that when you clear an array, it's most
    // likely you are going to put new stuff in.

    while(!IsEmpty()())
        Pop_back();
}

template <class T>
bool CBVector<T>::IsValidIndex(size_t index)
{
    return index >= 0 && index < m_Size;
}
// Insert a copy. And return ptr to the inserted copy.
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
    m_Data[m_Size - 1] = NULL;
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
