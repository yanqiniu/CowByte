#include "Window.h"
#include "../Utils/CBDebug.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_CREATE)
    {
        CREATESTRUCT *pCS = (CREATESTRUCT*)lParam;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pCS->lpCreateParams);
    }
    else
    {
        Window* pWindow = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (pWindow) return pWindow->HandleEvent(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_ACTIVATE:
    {
        if (!HIWORD(wParam))
            SetActive(true);
        else
            SetActive(false);
    }
    case WM_SIZE:
    {
        UINT width = LOWORD(lparam);
        UINT height = HIWORD(lparam);

        m_ResizeData.m_MustResize = true;
        m_ResizeData.m_NewWidth = m_Width;
        m_ResizeData.m_NewHeight = m_Height;

        return 0;
    }
    case WM_DISPLAYCHANGE:
    {
        InvalidateRect(hWnd, NULL, FALSE);

        return 0;
    }
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }
    }

    return DefWindowProc(hWnd, msg, wParam, lparam);
}

Window::Window(const WindowData& data) :
    System(data),
    m_hWindow(NULL),
    m_hDC(NULL),
    m_hInst(NULL),

    m_Height(data.m_Height),
    m_Width(data.m_Width),
    m_Bits(data.m_Bits),

    m_bFullScreen(data.m_isFullScreen),
    m_Title(data.m_Title),
    m_ResizeData()
{

}

Window::~Window()
{
}

bool Window::Initialize()
{
    System::Initialize();

    unsigned int pixelFormat;            // Holds the results after searching for a match.

    DWORD dwExStyle;                     // Windows Extended Style.
    DWORD dwStyle;                       // Windows Style.

    RECT wndRect;                        // Grabs Rectangle UR, LL values.
    wndRect.left = static_cast<long>(0);
    wndRect.right = static_cast<long>(m_Width);
    wndRect.top = static_cast<long>(0);
    wndRect.bottom = static_cast<long>(m_Height);

    WNDCLASSEX wndClass;                  // Windows Class Structure.
    std::tstring className = m_Title;   // Window title.

    ZeroMemory(&wndClass, sizeof(WNDCLASSEX));

    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_DBLCLKS | CS_DROPSHADOW | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = NULL;
    wndClass.cbWndExtra = NULL;
    wndClass.hInstance = (HINSTANCE)GetModuleHandle(NULL);
    wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);       // Set Icon.
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);     // Set Cursor.
    wndClass.hbrBackground = NULL;
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = className.c_str();

    if (!RegisterClassEx(&wndClass))
    {
        DbgERROR("Failed to register window");
        return false;
    }

    if (m_bFullScreen)
    {
        DEVMODE dmScreenSettings;
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsHeight = m_Height;
        dmScreenSettings.dmPelsWidth = m_Width;
        dmScreenSettings.dmBitsPerPel = m_Bits;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSHEIGHT | DM_PELSWIDTH;

        // Try to set selected mode and get results. Note: CDS_FULLSCREEN gets rid of start bar. 
        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            // If the mode fails, use windowed mode.
            DbgWARNING("The requested fullscreen mode is not suppoorted by your video card.");

            dwExStyle = WS_EX_APPWINDOW;
            dwStyle = WS_POPUP;
            //ShowCursor(false);
        }
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        dwStyle = WS_OVERLAPPEDWINDOW;
    }

    AdjustWindowRectEx(&wndRect, dwStyle, FALSE, dwExStyle);

    if (!(m_hWindow = CreateWindowEx( // Create the actual window.
        dwExStyle,
        className.c_str(),
        className.c_str(),
        dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        10,
        10,
        wndRect.right - wndRect.left,
        wndRect.bottom - wndRect.top,
        NULL,
        NULL,
        m_hInst,
        this
    )))
    {
        DbgERROR("Failed to window handle == NULL.");
        return false;
    }

    static PIXELFORMATDESCRIPTOR pfd =        // pfd tells windows how we want things to be
    {
        sizeof(PIXELFORMATDESCRIPTOR),        // Size of this pixel format descriptor.
        1,                                    // Version number.
        PFD_DRAW_TO_WINDOW |                  // Format must support window.
        PFD_SUPPORT_OPENGL |                  // Format must support opengl.
        //PFD_SUPPORT_DIRECTDRAW |
        PFD_DOUBLEBUFFER,                     // Must support double buffering.
        PFD_TYPE_RGBA,                        // Request an RGBA format.
        m_Bits,                               // Select out color depth.
        0, 0, 0, 0, 0, 0,                     // Color bits ignored.
        0,                                    // No alpha buffer.
        0,                                    // Shift bit ignored
        0,                                    // No accumulation buffer.
        0, 0, 0, 0,                           // Accumulation bits ignored.
        16,                                   // Depth of Z-Buffer
        0,                                    // No Stencil Buffer.
        0,                                    // No Auxiliary buffer.
        PFD_MAIN_PLANE,                       // Main Drawing Layer.
        0,                                    // Reserved.
        0, 0, 0                               // Layer mask ignored.
    };

    if (!(m_hDC = GetDC(m_hWindow)))          // Did we get a device context?
    {
        DbgERROR("Can't create device context.");
        return false;
    }

    if (!(pixelFormat = ChoosePixelFormat(m_hDC, &pfd)))
    {
        DbgERROR("Can't find a suitable pixel format.");
        return false;
    }

    if (!SetPixelFormat(m_hDC, pixelFormat, &pfd))
    {
        DbgERROR("Can't set the pixel format.");
        return false;
    }

    ShowWindow(m_hWindow, SW_SHOW);
    SetForegroundWindow(m_hWindow);
    SetFocus(m_hWindow);

    m_ResizeData.m_MustResize = true;
    m_ResizeData.m_NewWidth = m_Width;
    m_ResizeData.m_NewHeight = m_Height;

    if (!this->CenterWindow())
    {
        DbgERROR("Failed to center window.");
        return false;
    }

    // Disable closing button debug window. 
    HWND hConsoleWnd = GetConsoleWindow();
    if (hConsoleWnd != NULL)
    {
        HMENU hConsoleMenu = GetSystemMenu(hConsoleWnd, FALSE);
        if (hConsoleMenu != 0)
        {
            BOOL bRet = RemoveMenu(hConsoleMenu, SC_CLOSE, MF_BYCOMMAND);
            if (!bRet)
            {
                DbgERROR("Failed to remove close button.");
                return false;
            }
        }
    }

    DbgINFO("Window is initialized.");

    // Success:
    return true;
}

bool Window::Update(const GameContext& context)
{
    if(context.pWnd != this)
        const_cast<GameContext&>(context).pWnd = this;

    return true;
}

bool Window::ShutDown()
{
    if (m_bFullScreen)
    {
        ChangeDisplaySettings(NULL, 0);
        ShowCursor(true);
    }

    if (m_hDC && !ReleaseDC(m_hWindow, m_hDC))
    {
        DbgERROR("Release DC failed.");
        m_hDC = NULL;
        return false;
    }

    if (m_hWindow && !DestroyWindow(m_hWindow))
    {
        DbgERROR("Failed to destroy window.");
        m_hWindow = NULL;
        return false;
    }

    if (!UnregisterClass(m_Title.c_str(), m_hInst))
    {
        DbgERROR("Could not unregister class.");
        m_hInst = NULL;
        return false;
    }

    return true;
}

void Window::_HandleMessage(CBRefCountPtr<Message> pMsg)
{

}

bool Window::CenterWindow()
{
    return true;
}

WindowData::WindowData() :
    SystemData(SystemType::SYS_INVALID),
    m_Height(-1),
    m_Width(-1),
    m_Bits(-1),
    m_Title(_T("")),
    m_isFullScreen(false)
{

}

WindowData::WindowData(int w, int h, const std::tstring& title /*= _T("CowByte Engine Ver1.0 - by Roy Niu")*/, int b /*= 32*/, bool fullScreen /*= false*/) :
    SystemData(SystemType::SYS_WINDOW),
    m_Height(h),
    m_Width(w),
    m_Bits(b),
    m_Title(title),
    m_isFullScreen(fullScreen)

{

}

ResizeData::ResizeData() :
    m_MustResize(false),
    m_NewHeight(-1),
    m_NewWidth(-1)
{

}

ResizeData::ResizeData(bool resize, int nw, int nh) :
    m_MustResize(resize),
    m_NewHeight(nh),
    m_NewWidth(nw)
{

}
