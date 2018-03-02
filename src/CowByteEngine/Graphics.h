#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <D3DX10.h>
#include <D3DX11.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#include "System.h"
#include "Window.h"
#include "Mesh.h"



struct GraphicsData : SystemData
{
    GraphicsData();
    GraphicsData(Window *window);
    Window* m_pWindow;

};

class Graphics : public System
{
public:
    explicit Graphics( const GraphicsData &data);
    virtual ~Graphics();
    virtual bool Initialize();
    virtual bool Update(Context& context);
    virtual bool ShutDown();
    bool OnRender();

private:
    static const int NumOfBuffers = 2;

    D3D11_VIEWPORT m_Viewport;
    ID3D11Device *m_pDevice;
    ID3D11DeviceContext *m_pDeviceContext;
    IDXGISwapChain *m_pSwapChain;
    ID3D11RenderTargetView *m_pRenderTarget;
    ID3D11Buffer *m_pVertexBuffer;
    ID3D11InputLayout *m_pInputLayout;
    Window *m_pWindow;
    
};

#endif