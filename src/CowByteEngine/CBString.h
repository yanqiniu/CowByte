#ifndef _CBSTRING_H
#define _CBSTRING_H
#include <stdio.h>
#include <cstring>

template <size_t len>
class CBString
{
public:
    CBString();
    CBString(const char* cstring);
    ~CBString();

    void Set(const char *inStr);
    void Set(CBString &toAppend);
    char* Get();
    void Clear();
    size_t Length();

    void Append(const char *toAppend);
    void operator=(const char* cstring);

private:
    char m_Data[len];
    static char _s_buf[len];
};


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
    sprintf_s(m_Data, len, "%s", inStr);
}

template <size_t len>
inline void CBString<len>::Set(CBString &toAppend)
{
    Set(toAppend.Get());
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
inline size_t CBString<len>::Length()
{
    return strlen(m_Data);
}

template <size_t len>
inline void CBString<len>::Append(const char *toAppend)
{
    memcpy(m_Data + Length(), toAppend, strlen(toAppend) + 1);
}

template <size_t len>
inline void CBString<len>::operator=(const char* cstring)
{
    Set(cstring);
}

#endif // _COWSTRING_H

