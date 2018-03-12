#ifndef _TYPEDEFS_H
#define _TYPEDEFS_H

// Primitives ============================================
typedef unsigned int UINT32;
typedef long long INT64;
typedef int UID;

// String ================================================

// C Runtime Header Files
#ifndef _TCHAR_INC_
#include <tchar.h>
#endif

#ifndef _STRING_
#include <string>
#endif

// Defines
#ifdef UNICODE
#define tstring wstring
#define TOSTRING(value) std::to_wstring(value)
#else
#define tstring string
#define TOSTRING(value) std::to_string(value)

#endif


#endif // !_TYPEDEFS_H
