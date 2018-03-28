#ifndef _CBSTRING_H
#define _CBSTRING_H
#include <stdio.h>
#include <cstring>

#include "CBStringOps.h"

using namespace CBStringOps;
// CBE custom string object, as well as some
// functions that operate on cstrings.
template <size_t len>
class CBString
{
public:
    // TODO: for whatever reason this file doesn't let me include memory management files.

    CBString();
    CBString(const char* cstring);
    ~CBString();

    // member functions.
    void        Set(const char *inStr);
    void        Set(const CBString &toAppend);
    char*       Get();
    const char* Peek() const;
    void        Clear();
    size_t      Capacity() const;
    size_t      Size() const;
    int         Compare(const char *rhs) const;
    int         Compare(const CBString &rhs) const;
    void        Append(const char *toAppend);
    bool        Strip();
    bool        Strip(StripMode mode);

    void   operator=(const char* cstring);

    // static functions (CBString/cstring)


private:
    char m_Data[len];
    static char _s_buf[len];
};

// Get() but const.
template <size_t len>
inline const char* CBString<len>::Peek() const
{
    return m_Data;
}

template <size_t len>
inline int CBString<len>::Compare(const CBString &rhs) const
{
    return strcmp(m_Data, rhs.Peek());
}

template <size_t len>
inline int CBString<len>::Compare(const char *rhs) const
{
    return strcmp(m_Data, rhs);
}

template <size_t len>
inline size_t CBString<len>::Capacity() const
{
    return len;
}

template <size_t len>
inline bool CBString<len>::Strip(StripMode mode)
{
    return CBStringOps::Strip(m_Data, mode);
}

// Strip newline at end and then spaces at both ends.
template <size_t len>
bool CBString<len>::Strip()
{
    return Strip(StripMode::ALL);
}


template <size_t len> 
char CBString<len>::_s_buf[len];

template <size_t len>
CBString<len>::CBString()
{
}

template <size_t len>
CBString<len>::CBString(const char* cstring)
{
    Set(cstring);
}

template <size_t len>
CBString<len>::~CBString()
{
}

template <size_t len>
inline void CBString<len>::Set(const char *inStr)
{
    Clear();
    sprintf_s(m_Data, len, "%s", inStr);
}

template <size_t len>
inline void CBString<len>::Set(const CBString &value)
{
    Set(value.Peek());
}

template <size_t len>
inline char* CBString<len>::Get()
{
    return m_Data;
}

// Clear data and put null terminator in front.
template <size_t len>
inline void CBString<len>::Clear()
{
    memset(m_Data, 0, len);
    m_Data[0] = '\0';
}

template <size_t len>
inline size_t CBString<len>::Size() const
{
    return strlen(m_Data);
}

template <size_t len>
inline void CBString<len>::Append(const char *toAppend)
{
    memcpy(m_Data + Size(), toAppend, strlen(toAppend) + 1);
}


template <size_t len>
inline void CBString<len>::operator=(const char* cstring)
{
    Set(cstring);
}

#endif // _COWSTRING_H

