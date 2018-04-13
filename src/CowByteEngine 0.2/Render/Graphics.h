#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <windows.h>
#include <windowsx.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "../Core/System.h"
#include "MeshManager.h"
#include "GeometryGPU/TextureManager.h"
#include "GeometryGPU/LightManager.h"
#include "GeometryGPU/ConstantBufferGPU.h"

static const int g_ShadowMapW = 4096;
static const int g_ShadowMapH = 4096;

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
    virtual bool Update(const GameContext& context) override;
    virtual bool ShutDown();
    //void SetCamera();;

private:
    bool PassDepthOnly();
    bool PassShadowMap();
    bool PassDraw();

    bool OnRender(UINT numIndices);
    void _HandleMessage(CBRefCountPtr<Message> &pMsg) override;
    bool InitializePipeline();

    static const int NumOfBuffers = 1;
    enum ConstantBufferType
    {
        CBUFFER_APPLICATION,
        CBUFFER_FRAME,
        CBUFFER_OBJECT,
        NUM_CBUFFER
    };

    // Constant buffers:
    PerFrameConstBufGPU  m_PerFrameConstBuf;
    PerObjectConstBufGPU m_PerObjectConstBuf;

    // Ptrs to the necessary three:
    ID3D11Device        *m_pDevice;
    ID3D11DeviceContext *m_pDeviceContext;
    IDXGISwapChain      *m_pSwapChain;

    // Render target buffer and views, as well as shader resource views sharing the resource.
    ID3D11Texture2D          *m_pZBuffer;
    ID3D11Texture2D          *m_pShadowMap;
    ID3D11RenderTargetView   *m_pRenderTargetView;
    ID3D11RenderTargetView   *m_pZBufferRTView;
    ID3D11ShaderResourceView *m_pZBufferSRView;
    ID3D11RenderTargetView   *m_pShadowRTView;
    ID3D11ShaderResourceView *m_pShadowSRView;

    // Depth stencil buffers, views and states.
    ID3D11Texture2D          *m_pDepthStencilBuffer;
    ID3D11Texture2D          *m_pDepthStencilBufferNoMS;
    ID3D11Texture2D          *m_pDepthStencilBufferShadow;
    ID3D11DepthStencilView   *m_pDepthStencilView;
    ID3D11DepthStencilView   *m_pDepthStencilViewNoMS;
    ID3D11DepthStencilView   *m_pDepthStencilViewShadow;
    ID3D11DepthStencilState  *m_pDepthStencilState;
    ID3D11DepthStencilState  *m_pDepthStencilStateNoMS;
    ID3D11DepthStencilState  *m_pDepthStencilStateShadow;

    // View port:
    D3D11_VIEWPORT m_MainViewport;
    D3D11_VIEWPORT m_ShadowViewport;

    // Shaders, sampler states:
    ID3D11InputLayout        *m_pDepthInputLayout;
    ID3D11VertexShader       *m_pDepthOnlyVS;
    ID3D11VertexShader       *m_pShadowMapVS;
    ID3D11PixelShader        *m_pDepthOnlyPS;
    ID3D11SamplerState       *m_pPointSS;




    // Other members:
    Window *m_pWindow;
    MeshManager *m_pMeshManager;
    TextureManager *m_pTexManager;
    LightManager *m_pLightManager;
    Camera *m_pMainCamera;
    UID m_LastDrawnMeshID;
};

#endif