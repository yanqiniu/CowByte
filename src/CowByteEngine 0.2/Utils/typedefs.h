#ifndef _TYPEDEFS_H
#define _TYPEDEFS_H

#include "CBString.h"

// Primitives ============================================
typedef unsigned int UINT32;
typedef long long INT64;
typedef int UID;
typedef CBString<256> Filepath;
typedef CBString<64> Filename;

#define INVALID_UID -1

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
