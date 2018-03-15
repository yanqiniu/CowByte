
#ifndef _WINDOW_H
#define _WINDOW_H

#include <Windows.h>

#include "../Core/System.h"
#include "../Core/GameContext.h"
#include "../Utils/typedefs.h"


struct WindowData : public SystemData
{
    WindowData();
    WindowData(int w, int h, const std::tstring& title = _T("CowByte Engine Ver 0.2 - Roy Niu"), int b = 32, bool fullScreen = false);

    int m_Width;
    int m_Height;
    int m_Bits;
    std::tstring m_Title;
    bool m_isFullScreen;
};

struct ResizeData
{
    ResizeData();
    ResizeData(bool resize, int nw, int nh);

    bool m_MustResize;
    int m_NewWidth;
    int m_NewHeight;
};

class Window : public System
{
    friend class Engine;
public:
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    HWND GetWindowHandle() const { return m_hWindow; }
    bool GetIsFullScreen() const { return m_bFullScreen; }
    HDC GetDeviceContext();
    HINSTANCE GetInstance();

    ResizeData& GetResizeData() { return m_ResizeData; }

    LRESULT HandleEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lparam);
    
protected:
    Window(const WindowData& data);
    virtual ~Window();

    virtual bool Initialize();
    virtual bool Update(const GameContext &context) override;
    virtual bool ShutDown();
    virtual void _HandleMessage(CBRefCountPtr<Message> &pMsg) override;
private:
    bool CenterWindow();

    HWND m_hWindow;
    HDC m_hDC;
    HINSTANCE m_hInst;

    int m_Width;
    int m_Height;

    int m_Bits;
    bool m_bFullScreen;
    std::tstring m_Title;
    ResizeData m_ResizeData;
};

#endif
