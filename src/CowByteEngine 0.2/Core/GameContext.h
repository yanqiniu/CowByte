#ifndef _CONTEXT_H
#define _CONTEXT_H

//////////////////////////////////////////////////////////////////////////

class Window;
class MessageBus;

//////////////////////////////////////////////////////////////////////////


struct GameContext
{
    Window* pWnd;
    float dTime; //deltaTime
    GameContext() :
        pWnd(nullptr),
        dTime(0)
    {

    }
};

#endif // 
