#ifndef _LIGHT_H
#define _LIGHT_H
#include "../Other/CBColor.h"

class Light
{
public:
    Light();
    virtual ~Light();

protected:
    CBColor m_Color;
};
#endif // _LIGHT_H

