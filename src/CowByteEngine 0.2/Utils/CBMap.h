#ifndef _CBMAP_H
#define _CBMAP_H
#include "../Memory/CBMemory.h"
#include "../Utils/typedefs.h"

// A simple map that only takes UID as keys.
// Notice that this has to be efficient and fast,
// so be sure to initialize it to a large capacity 
// to avoid collision.
// Also, notice that this doesn't grow, so be sure 
// to have a big enough one.
template<typename T>
struct MapBlock
{
    CBMEM_OVERLOAD_NEW_DELETE(MapBlock);

    MapBlock(): isEmpty(true), key(INVALID_UID) {}

    union
    {
        T _data;
        bool isEmpty;
    };
    UID key;
};


template<typename T>
class CBMap
{
public:

    template<typename T>

    CBMEM_OVERLOAD_NEW_DELETE(CBMap);

    explicit CBMap(size_t strtCap);

    T* Insert(const T& toInsert, UID id);
    T* Get(UID id);
    size_t Capacity() const { return m_Capacity; }
    const T* Peek(UID id) const;

private:
    size_t UIDToIndex(UID id) const;

    MapBlock<T> *m_Data;
    size_t m_Capacity;
};

// Initialize data array to startCp capacity.
// Make sure this enough to reduce collision.
template<typename T>
CBMap<T>::CBMap(size_t strtCap)
{
    m_Data = new MapBlock<T>();
    m_Capacity = strtCap;

}

// Hash function.
template<typename T>
size_t CBMap<T>::UIDToIndex(UID id) const
{
    return id % m_Capacity;
}

template<typename T>
T* CBMap<T>::Insert(const T& toInsert, UID id)
{
    size_t index = UIDToIndex(id);
    if (m_Data[index].isEmpty)
    {
        m_Data[index].key = id;
        m_Data[index]._data = toInsert;
        return &m_Data[index]._data;
    }
    else // Hash collision
    {
        // Linear probing:
        for (size_t i = 0; i < m_Capacity; ++i)
        {
            if (m_Data[i].isEmpty)
            {
                m_Data[i].key = id;
                m_Data[i]._data = toInsert;
                return &m_Data[index]._data;
            }
        }

        // Map is full.
        return nullptr;
    }
}

template<typename T>
T* CBMap<T>::Get(UID id)
{
    size_t index = UIDToIndex(id);
    if (m_Data[index].key == id)
    {
        return &m_Data[index]._data;
    }
    else // there was a collision or empty.
    {
        // Linear probing:
        for (size_t i = 0; i < m_Capacity; ++i)
        {
            if (m_Data[i].key == id)
            {
                return &m_Data[i]._data;
            }
        }

        // Did not find.
        return nullptr;
    }
}

template<typename T>
const T* CBMap<T>::Peek(UID id) const
{
    size_t index = UIDToIndex(id);
    if (m_Data[index].key == id)
    {
        return &m_Data[index]._data;
    }
    else // there was a collision or empty.
    {
        // Linear probing:
        for (size_t i = 0; i < m_Capacity; ++i)
        {
            if (m_Data[i].key == id)
            {
                return &m_Data[i]._data;
            }
        }

        // Did not find.
        return nullptr;
    }
}


#endif // _CBMAP_H
