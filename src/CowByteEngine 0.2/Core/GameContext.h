#ifndef _CONTEXT_H
#define _CONTEXT_H
#include "../Memory/CBMemory.h"

//////////////////////////////////////////////////////////////////////////

class Window;
class MessageBus;

//////////////////////////////////////////////////////////////////////////


struct GameContext
{
    CBMEM_OVERLOAD_NEW_DELETE(GameContext)
    Window* pWnd;
    float dTime; //deltaTime
    GameContext() :
        pWnd(nullptr),
        dTime(0)
    {

    }
};

#endif // 
