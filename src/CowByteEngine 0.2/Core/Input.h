#ifndef _CBINPUT_H
#define _CBINPUT_H

#include <dinput.h>
#include "System.h"
#include "../Render/Window.h"
#include "../Utils/typedefs.h"
#include "../Utils/KeyCodes.h"
#include "../Math/Point.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

//////////////////////////////////////////////////////////////////////////

class InputStatus
{
    friend class Input;
public:
    InputStatus() :
        m_Keys{ 0 },
        m_MouseDelta()
    {

    }

    ~InputStatus() {};

    // Check is key is down this frame.
    bool GetKeyDown(KeyCodes keyCode) const
    {
        size_t index = static_cast<KeyCodes>(keyCode);
        return m_Keys[index] == 0x80;
    }

    Point GetMouseDelta()
    {
        return m_MouseDelta;
    }

private:
    unsigned char m_Keys[256];
    Point m_MouseDelta;
};

//////////////////////////////////////////////////////////////////////////

struct InputSystemData : public SystemData
{
    InputSystemData();
    InputSystemData(Window *window);
    Window* m_pWindow;
};


class Input : public System
{
    friend class InputStatus;
public:
    Input(const InputSystemData &data);
    virtual ~Input();

    bool Initialize();
    bool Update(const GameContext &context) override;
    virtual bool Shutdown();
    void _HandleMessage(CBRefCountPtr<Message> &pMsg) override;

    bool GetKeyPressed(KeyCodes keyCode);  // Key was up last frame and is down this frame.
    bool GetKeyUp(KeyCodes keyCode);    // Key was down last frame and is up this frame.
    bool GetKeyHeld(KeyCodes keyCode);  // Key was down both last frame and this frame.

private:
    int ReadKeyboard();
    int ReadMouse();
    void ProcessKeyboardInput();
    void ProcessMouseInput();

    InputStatus m_InputCur;
    InputStatus m_InputPrev;

    unsigned char m_KeyboardState[256];
    DIMOUSESTATE m_MouseState;
    int m_WindowWidth, m_WindowHeight;

    IDirectInput8*       m_pDirectInput;
    IDirectInputDevice8* m_pKeyboard;
    IDirectInputDevice8* m_pMouse;

    Window *m_pWindow;
};

#endif


