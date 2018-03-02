#ifndef _COWSTRING_H
#define _COWSTRING_H
#include <stdio.h>
#include <cstring>

// Use only for stuff long enough...like a path.
template <size_t len>
class CowString
{
public:
    CowString();
    CowString(const char* cstring);
    virtual ~CowString();

    void Set(const char *inStr);
    void Set(CowString &toAppend);
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
char CowString<len>::_s_buf[len];

template <size_t len>
CowString<len>::CowString()
{
}

template <size_t len>
CowString<len>::CowString(const char* cstring)
{
    Set(cstring);
}

template <size_t len>
CowString<len>::~CowString()
{
}

template <size_t len>
inline void CowString<len>::Set(const char *inStr)
{
    sprintf_s(m_Data, len, "%s", inStr);
}

template <size_t len>
inline void CowString<len>::Set(CowString &toAppend)
{
    Set(toAppend.Get());
}

template <size_t len>
inline char* CowString<len>::Get()
{
    return m_Data;
}

// Clear data and put null terminator in front.
template <size_t len>
inline void CowString<len>::Clear()
{
    memset(m_Data, 0, len);
    m_Data[0] = '\0';
}

template <size_t len>
inline size_t CowString<len>::Length()
{
    return strlen(m_Data);
}

template <size_t len>
inline void CowString<len>::Append(const char *toAppend)
{
    memcpy(m_Data + Length(), toAppend, strlen(toAppend) + 1);
}

template <size_t len>
inline void CowString<len>::operator=(const char* cstring)
{
    Set(cstring);
}

#endif // _COWSTRING_H

