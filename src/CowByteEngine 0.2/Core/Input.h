#ifndef _CBINPUT_H
#define _CBINPUT_H

#include <dinput.h>
#include "System.h"
#include "../Render/Window.h"
#include "../Utils/typedefs.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// TODO: move this to a more proper place.
struct Point
{
    INT32 X;
    INT32 Y;

    Point() :
        X(0),
        Y(0)
    {

    }

    friend bool operator==(const Point& p1, const Point& p2)
    {
        return p1.X == p2.X && p1.Y == p2.Y;
    }

    friend bool operator!=(const Point& p1, const Point& p2)
    {
        return p1.X != p2.X || p1.Y != p2.Y;
    }

};

struct InputStatus
{
    unsigned char Keys[256];

    Point MousePos;
};

struct InputSystemData : public SystemData
{
    InputSystemData();
    InputSystemData(Window *window);
    Window* m_pWindow;
};


class Input : public System
{
public:
    Input(const InputSystemData &data);
    virtual ~Input();

    bool Initialize();
    bool Update(const GameContext &context) override;
    virtual bool Shutdown();

    void _HandleMessage(CBRefCountPtr<Message> &pMsg) override;

private:
    bool ReadKeyboard();
    bool ReadMouse();
    void ProcessDirectInput();

    InputStatus m_CurInput;
    InputStatus m_PrevInput;

    unsigned char m_KeyboardState[256];
    DIMOUSESTATE m_MouseState;
    int m_MouseRawX, m_MouseRawY; // Un-clamped mouse pos value.
    int m_WindowWidth, m_WindowHeight;

    IDirectInput8*       m_pDirectInput;
    IDirectInputDevice8* m_pKeyboard;
    IDirectInputDevice8* m_pMouse;

    Window *m_pWindow;
};

#endif


