#include "Input.h"
#include "../Utils/exceptions.h"
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
    m_WindowHeight(data.m_pWindow->GetHeight()),
    m_MouseRawX(0),
    m_MouseRawY(0)
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
    ThrowIfFailed(m_pKeyboard->Acquire());

    // Setup mouse.
    ThrowIfFailed(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr));
    ThrowIfFailed(m_pMouse->SetDataFormat(&c_dfDIMouse));
    ThrowIfFailed(m_pMouse->SetCooperativeLevel(m_pWindow->GetWindowHandle(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)); // Have to check for when it goes in and out of focus and re-acquire it each time.
    ThrowIfFailed(m_pMouse->Acquire());

    return true;
}

bool Input::Update(const GameContext &context)
{
    // Read the current state of the keyboard.
    if (!ReadKeyboard())
        return false;

    // Read the current state of the mouse.
    if (!ReadMouse())
        return false;

    // Process the changes in the mouse and keyboard.
    ProcessDirectInput();

    for (int i = 0; i < 256; ++i)
    {
        if (m_CurInput.Keys[i] == 0x80)
        {
            DbgINFO("Key[%#04x] is down!", i);
        }
    }


    if (m_CurInput.MousePos != m_PrevInput.MousePos)
    {
        DbgINFO("MousPos<%d, %d>!", m_MouseState.lX, m_MouseState.lY);

    }

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

bool Input::ReadKeyboard()
{
    HRESULT result = m_pKeyboard->GetDeviceState(sizeof(m_KeyboardState), (LPVOID)&m_KeyboardState);
    if (FAILED(result))
    {
        // If the keyboard lost focus or was not acquired then try to get it back.
        if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            m_pKeyboard->Acquire();
        }
        else
        {
            DbgERROR("Failed getting keyboard state!");
            return false;
        }
    }
    return true;
}

bool Input::ReadMouse()
{
    HRESULT result = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_MouseState);
    if (FAILED(result))
    {
        // If the keyboard lost focus or was not acquired then try to get it back.
        if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            m_pMouse->Acquire();
        }
        else
        {
            DbgERROR("Failed getting mouse state!");
            return false;
        }
    }
    return true;

}

void Input::ProcessDirectInput()
{
    m_PrevInput = m_CurInput;

    m_MouseRawX += m_MouseState.lX;
    m_MouseRawY += m_MouseState.lY;

    m_CurInput.MousePos.X = CBHELPER_CLAMP(m_MouseRawX, 0, m_WindowWidth);
    m_CurInput.MousePos.Y = CBHELPER_CLAMP(m_MouseRawY, 0, m_WindowHeight);

    memcpy(m_CurInput.Keys, m_KeyboardState, sizeof(m_CurInput.Keys));
}
