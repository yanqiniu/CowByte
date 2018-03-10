#ifndef _CONTEXT_H
#define _CONTEXT_H

//////////////////////////////////////////////////////////////////////////

class Window;
class MessageBus;

//////////////////////////////////////////////////////////////////////////


struct GameContext
{
    Window* pWnd;
    MessageBus* pEngineMessageBus;
    float dTime; //deltaTime
};

#endif // 