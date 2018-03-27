#include "Input.h"
#include "../Utils/CBDebug.h"
#include "../Utils/CBHelper.h"


using namespace DirectX;

InputSystemData::InputSystemData() :
    m_pWindow(nullptr),
    SystemData(SystemType::SYS_INPUT)
{

}

InputSystemData::InputSystemData(Window *window) :
    m_pWindow(window),
    SystemData(SystemType::SYS_INPUT)
{

}


Input::Input(const InputSystemData &data) :
    System(data),
    m_pWindow(data.m_pWindow),
    m_KeyboardState(),
    m_MouseState(),
    m_WindowWidth(data.m_pWindow->GetWidth()),
    m_WindowHeight(data.m_pWindow->GetHeight())
{

}

Input::~Input()
{

}

bool Input::Initialize()
{
    ThrowIfFailed(DirectInput8Create(m_pWindow->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL));
 
    // Setup keyboard.
    ThrowIfFailed(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, nullptr));
    ThrowIfFailed(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard));
    ThrowIfFailed(m_pKeyboard->SetCooperativeLevel(m_pWindow->GetWindowHandle(), DISCL_EXCLUSIVE | DISCL_FOREGROUND)); // Do not share with other programs.

    // Setup mouse.
    ThrowIfFailed(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr));
    ThrowIfFailed(m_pMouse->SetDataFormat(&c_dfDIMouse));
    ThrowIfFailed(m_pMouse->SetCooperativeLevel(m_pWindow->GetWindowHandle(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)); // Have to check for when it goes in and out of focus and re-acquire it each time.

    return true;
}

bool Input::Update(const GameContext &context)
{
    int keyRes = ReadKeyboard();
    int mouseRes = ReadMouse();

    if (keyRes == 0)
        ProcessKeyboardInput();
    if (mouseRes == 0)
        ProcessMouseInput();

    if (keyRes == 1 || mouseRes == 1)
        return false;

    return true;
}

bool Input::Shutdown()
{
    if (m_pMouse != nullptr)
    {
        m_pMouse->Unacquire();
        m_pMouse->Release();
        m_pMouse = nullptr;
    }

    if (m_pKeyboard != nullptr)
    {
        m_pKeyboard->Unacquire();
        m_pKeyboard->Release();
        m_pKeyboard = nullptr;
    }

    if (m_pDirectInput != nullptr)
    {
        m_pDirectInput->Release();
        m_pDirectInput = nullptr;
    }

    bool toRet = true;
    toRet &= System::Shutdown();

    return toRet;

}

void Input::_HandleMessage(CBRefCountPtr<Message> &pMsg)
{

}

// Key was up last frame and is down this frame.
bool Input::GetKeyPressed(KeyCodes keyCode)
{
    return !m_InputPrev.GetKeyDown(keyCode) && m_InputCur.GetKeyDown(keyCode);
}

// Key was down last frame and is up this frame.
bool Input::GetKeyUp(KeyCodes keyCode)
{
    return m_InputPrev.GetKeyDown(keyCode) && !m_InputCur.GetKeyDown(keyCode);
}

// Key was down both last frame and this frame.
bool Input::GetKeyHeld(KeyCodes keyCode)
{
    return m_InputPrev.GetKeyDown(keyCode) && m_InputCur.GetKeyDown(keyCode);
}

// 0: Succeeded.
// 1: Failed.
// 2: Attempted reacquire.
int Input::ReadKeyboard()
{
    HRESULT result = m_pKeyboard->GetDeviceState(sizeof(m_KeyboardState), (LPVOID)&m_KeyboardState);
    if (FAILED(result))
    {
        // If the keyboard lost focus or was not acquired then try to get it back.
        if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            // Acquire then try to read again next frame.
            m_pKeyboard->Acquire();
            return 2;
        }
        else
        {
            DbgERROR("Failed getting keyboard state!");
            return 1;
        }
    }
    return 0;
}

// 0: Succeeded.
// 1: Failed.
// 2: Attempted reacquire.
int Input::ReadMouse()
{
    HRESULT result = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_MouseState);
    if (FAILED(result))
    {
        // If the keyboard lost focus or was not acquired then try to get it back.
        if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            // Acquire then try to read again next frame.
            m_pMouse->Acquire();
            return 2;
        }
        else
        {
            DbgERROR("Failed getting mouse state!");
            return 1;
        }
    }
    return 0;
}

void Input::ProcessKeyboardInput()
{
    memcpy(m_InputCur.m_Keys, m_KeyboardState, sizeof(m_InputCur.m_Keys));
}

void Input::ProcessMouseInput()
{
    m_InputPrev = m_InputCur;
    m_InputCur.m_MouseDelta.X = m_MouseState.lX;
    m_InputCur.m_MouseDelta.Y = m_MouseState.lY;
}


