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
    bool PassDepthOnly();
    bool OnRender(UINT numIndices);
    bool PassDraw();
    //void SetCamera();;

private:
    void _HandleMessage(CBRefCountPtr<Message> &pMsg) override;
    bool InitializePipeline();

    static const int NumOfBuffers = 2;
    enum ConstantBufferType
    {
        CBUFFER_APPLICATION,
        CBUFFER_FRAME,
        CBUFFER_OBJECT,
        NUM_CBUFFER
    };

    // Constant buffers:
    PerFrameConstBufGPU m_PerFrameConstBuf;
    PerObjectConstBufGPU m_PerObjectConstBuf;

    // View port and other large member:
    D3D11_VIEWPORT m_Viewport;

    // Ptr to the necessary three:
    ID3D11Device *m_pDevice;
    ID3D11DeviceContext *m_pDeviceContext;
    IDXGISwapChain *m_pSwapChain;

    // Other graphics:
    ID3D11RenderTargetView *m_pRenderTargetView;
    ID3D11DepthStencilView* m_pDepthStencilView;

    // ZBuffer:
    ID3D11VertexShader       *m_pDepthOnlyVS;
    ID3D11PixelShader        *m_pDepthOnlyPS;
    ID3D11Texture2D          *m_pZBuffer;
    ID3D11RenderTargetView   *m_pZBufferView;
    ID3D11ShaderResourceView *m_pZBufferRscView;
    ID3D11SamplerState       *m_pZBufferSS;


    ID3D11Texture2D* m_pDepthStencilBuffer;
    ID3D11DepthStencilState* m_pDepthStencilState;
    ID3D11RasterizerState* m_pRasterizerState;


    // Other members:
    Window *m_pWindow;
    MeshManager *m_pMeshManager;
    TextureManager *m_pTexManager;
    LightManager *m_pLightManager;
    Camera *m_pMainCamera;
    UID m_LastDrawnMeshID;
};

#endif