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


struct GraphicsData : SystemData
{
    explicit GraphicsData();
};

class Graphics : public System
{
public:
    Graphics();
    virtual ~Graphics();
    virtual bool Initialize();
    virtual bool Update(Context& context);
    virtual bool ShutDown();

private:
    ID3D11Device *m_Device;
    ID3D11DeviceContext *m_DeviceContext;
    IDXGISwapChain *m_SwapChain;



};

#endif