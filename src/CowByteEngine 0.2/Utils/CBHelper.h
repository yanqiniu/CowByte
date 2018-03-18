// Handy helper macro/functions that don't really fit elsewhere.

#ifndef _CBHELPER_H
#define _CBHELPER_H

#define CBHELPER_TOSTR(x) #x

#define CBHELPER_FLOOR_CHECK(inVal, floorVal) ((inVal) < (floorVal) ? (floorVal) : (inVal))
#define CBHELPER_CEIL_CHECK(inVal, ceilVal)  ((inVal) > (ceilVal) ? (ceilVal) : (inVal))
#define CBHELPER_CLAMP(inVal, lowBound, highBound)  (CBHELPER_FLOOR_CHECK(CBHELPER_CEIL_CHECK(inVal, highBound), lowBound))

namespace CBHelper
{
}

#endif