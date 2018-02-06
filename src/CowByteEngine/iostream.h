#ifndef _IOSTREAM_H
#define _IOSTREAM_H

// C Runtime Header Files

#ifndef _IOSTREAM_
#include <iostream>
#endif

// Defines
#ifdef UNICODE
    #define tcout wcout
    #define tcin wcin
#else
    #define tcout cout
    #define tcin cin
#endif // UNICODE


#endif // _IOSTREAM_H