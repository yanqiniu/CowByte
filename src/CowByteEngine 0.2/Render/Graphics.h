#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <D3DX10.h>
#include <D3DX11.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "../Core/System.h"
#include "MeshManager.h"

class Window;
class Camera;

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
    virtual bool Update(GameContext& context);
    virtual bool ShutDown();
    bool OnRender();
    //void SetCamera();

private:
    bool InitializePipeline();
    bool SimpleRenderSetup(); // TODO: Get rid of this placeholder code piece.

    static const int NumOfBuffers = 2;
    enum ConstantBufferType
    {
        CBUFFER_APPLICATION,
        CBUFFER_FRAME,
        CBUFFER_OBJECT,
        NUM_CBUFFER
    };

    // View port and other large member:
    D3D11_VIEWPORT m_Viewport;

    // Ptr to the necessary three:
    ID3D11Device *m_pDevice;
    ID3D11DeviceContext *m_pDeviceContext;
    IDXGISwapChain *m_pSwapChain;

    // Other graphics:
    ID3D11RenderTargetView *m_pRenderTarget;
    ID3D11DepthStencilView* m_pDepthStencilView;

    ID3D11Buffer *m_pVertexBuffer;
    ID3D11Buffer *m_pIndexBuffer;
    ID3D11Buffer *m_pConstantBuffers[NUM_CBUFFER];
    ID3D11Texture2D* m_pDepthStencilBuffer;
    ID3D11InputLayout *m_pInputLayout;
    ID3D11DepthStencilState* m_pDepthStencilState;
    ID3D11RasterizerState* m_pRasterizerState;

    // Other members:
    Window *m_pWindow;
    MeshManager *m_pMeshManager;
    Camera *m_pMainCamera;


};

#endif