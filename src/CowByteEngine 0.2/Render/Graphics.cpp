#include <exception>
#include <DirectXMath.h>

#include "Graphics.h"
#include "Window.h"
#include "GeometryCPU/Vertex.h"
#include "../Math/Vec3.h"
#include "../Utils/CBString.h"
#include "../Utils/CBPath.h"
#include "../Utils/CBDebug.h"
#include "../Utils/typedefs.h"
#include "../SceneGraph/Camera.h"
#include "../SceneGraph/SceneNode.h"
#include "GeometryCPU/MeshInstance.h"
#include "../Utils/CBFile.h"
#include "../Messaging/CBMessaging.h"
#include "GeometryGPU/GPURegisterLayout.h"

#include "DDSTextureLoader/DDSTextureLoader.h"


using namespace DirectX;

GraphicsData::GraphicsData() :
    SystemData(SYS_GRAPHICS),
    m_pWindow(nullptr)
{

}


GraphicsData::GraphicsData(Window *window) :
    SystemData(SYS_GRAPHICS),
    m_pWindow(window)

{

}

Graphics::Graphics(const GraphicsData &data):
    System(data),
    m_pWindow(data.m_pWindow),
    m_PerFrameConstBuf(),
    m_PerObjectConstBuf(),
    m_pDevice(nullptr),
    m_pDeviceContext(nullptr),
    m_pSwapChain(nullptr),
    m_pMeshManager(nullptr),
    m_pTexManager(nullptr),
    m_pDepthStencilView(nullptr),
    m_pDepthStencilBuffer(nullptr),
    m_pDepthStencilState(nullptr),
    m_pPointSS(nullptr),
    m_LastDrawnMeshID(INVALID_UID)
{

}

Graphics::~Graphics()
{
}

bool Graphics::Initialize()
{
    if (m_pWindow == nullptr)
    {
        DbgERROR("Graphics: Window pointer null!");
        return false;
    }
    System::Initialize();

    if (!InitializePipeline())
    {
        DbgERROR("Graphics: Failed initialize pipeline!");
        return false;
    }

    // Setup mesh manager and load meshes.
    m_pMeshManager = new MeshManager();
    m_pMeshManager->AttachTo_NonSceneNode_Parent(this);
    if(!m_pMeshManager->CPULoadMesh("cube.mesha") ||
        !m_pMeshManager->CPULoadMesh("plane.mesha") ||
        !m_pMeshManager->CPULoadMesh("cow.mesha") ||
        !m_pMeshManager->CPULoadMesh("sphere.mesha")
        )
    {
        return false;
    }

    // Setup texture manager and load meshes on GPU.
    m_pTexManager = new TextureManager();
    m_pTexManager->AttachTo_NonSceneNode_Parent(this);
    if (!m_pMeshManager->GPULoadMeshes(m_pDevice, m_pDeviceContext, m_pTexManager))
        return false;

    // Setup Light Manager.
    m_pLightManager = new LightManager();
    m_pLightManager->AttachTo_NonSceneNode_Parent(this);

    return true;
}

bool Graphics::Update(const GameContext& context)
{

    if (m_pMainCamera == nullptr)
    {
        DbgWARNING("No main camera set, send register camera message!");
        return false;
    }
    else
    {
        // Update camera ViewProjection Matrix as well as position.
        m_pMainCamera->UpdateVPMatrix();
        m_pDeviceContext->UpdateSubresource(m_PerFrameConstBuf.m_ViewProjMatrix, 0, nullptr, &m_pMainCamera->GetViewProjMatrix(), 0, 0);
        m_pDeviceContext->UpdateSubresource(m_PerFrameConstBuf.m_CameraWorldPos, 0, nullptr, &m_pMainCamera->GetParentSceneNode()->GetWorldTransform().GetPosition(), 0, 0);
    }

    m_pLightManager->CreateLightsGPU(m_pDevice, m_pDeviceContext); // Don't worry this only run once.
    m_pLightManager->UpdateLightsGPU(m_pDeviceContext, &m_PerFrameConstBuf);

    // Draw passes.
    PassDepthOnly();
    PassShadowMap();
    PassDraw();

    m_pSwapChain->Present(0, 0);
    return true;
}

bool Graphics::ShutDown()
{
    m_pSwapChain->Release();
    m_pDevice->Release();
    m_pDeviceContext->Release();
    m_pMeshManager->ReleaseMeshesGPU();
    m_pTexManager->Release();
    Component::Shutdown();
    return true;
}

bool Graphics::PassDepthOnly()
{
    FLOAT color = m_pMainCamera->GetFarPlane(); // Use this since we record z'z in depth buffer, instead of z'.
    m_pDeviceContext->OMSetRenderTargets(1, &m_pZBufferRTView, m_pDepthStencilViewNoMS);
    m_pDeviceContext->ClearRenderTargetView(m_pZBufferRTView, &color);
    m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilViewNoMS, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);

    m_pDeviceContext->VSSetShader(m_pDepthOnlyVS, nullptr, 0);
    m_pDeviceContext->PSSetShader(m_pDepthOnlyPS, nullptr, 0);
    m_pDeviceContext->IASetInputLayout(m_pDepthInputLayout);

    m_pDeviceContext->RSSetViewports(1, &m_MainViewport);

    for (int i = 0; i < m_pMeshManager->GetMeshInsts().Size(); ++i)
    {
        MeshInstance *pMeshInst = m_pMeshManager->GetMeshInsts().peekat(i);
        Mesh* mesh = m_pMeshManager->GetMeshPtr(pMeshInst->GetMeshID());
        if (mesh->IsLoaded())
        {
            mesh->GetVertexBuffer().SetAsActive(m_pDeviceContext);
            mesh->GetIndexBuffer().SetAsActive(m_pDeviceContext);
        }

        if (mesh == nullptr)
            continue;

        // Update world matrix.
        m_pDeviceContext->UpdateSubresource(m_PerObjectConstBuf.m_WorldMatrix, 0, nullptr, &pMeshInst->GetParentSceneNode()->GetWorldTransform(), 0, 0);
        m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_pDeviceContext->DrawIndexed(mesh->GetIndexBuffer().Count(), 0, 0);
    }


    return true;
}

bool Graphics::PassShadowMap()
{
    FLOAT color = m_pMainCamera->GetFarPlane(); // Use this since we record z'z in depth buffer, instead of z'.
    m_pDeviceContext->OMSetRenderTargets(1, &m_pShadowRTView, m_pDepthStencilViewShadow);
    m_pDeviceContext->ClearRenderTargetView(m_pShadowRTView, &color);
    m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilStateShadow, 1);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilViewShadow, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);

    m_pDeviceContext->VSSetShader(m_pShadowMapVS, nullptr, 0);
    m_pDeviceContext->PSSetShader(m_pDepthOnlyPS, nullptr, 0);
    m_pDeviceContext->IASetInputLayout(m_pDepthInputLayout);

    m_pDeviceContext->RSSetViewports(1, &m_ShadowViewport);

    for (int i = 0; i < m_pMeshManager->GetMeshInsts().Size(); ++i)
    {
        MeshInstance *pMeshInst = m_pMeshManager->GetMeshInsts().peekat(i);
        Mesh* mesh = m_pMeshManager->GetMeshPtr(pMeshInst->GetMeshID());
        if (mesh->IsLoaded())
        {
            mesh->GetVertexBuffer().SetAsActive(m_pDeviceContext);
            mesh->GetIndexBuffer().SetAsActive(m_pDeviceContext);
        }

        if (mesh == nullptr)
            continue;

        // Update world matrix.
        m_pDeviceContext->UpdateSubresource(m_PerObjectConstBuf.m_WorldMatrix, 0, nullptr, &pMeshInst->GetParentSceneNode()->GetWorldTransform(), 0, 0);
        m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_pDeviceContext->DrawIndexed(mesh->GetIndexBuffer().Count(), 0, 0);
    }


    return true;

}

bool Graphics::OnRender(UINT numIndices)
{
    return true;
}

bool Graphics::PassDraw()
{
    FLOAT color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
    m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);

    m_pDeviceContext->RSSetViewports(1, &m_MainViewport);
    for (int i = 0; i < m_pMeshManager->GetMeshInsts().Size(); ++i)
    {
        MeshInstance *pMeshInst = m_pMeshManager->GetMeshInsts().peekat(i);
        Mesh* mesh = m_pMeshManager->GetMeshPtr(pMeshInst->GetMeshID());;
        if (mesh->IsLoaded())
        {
            mesh->GetVertexBuffer().SetAsActive(m_pDeviceContext);
            mesh->GetIndexBuffer().SetAsActive(m_pDeviceContext);
            mesh->GetMaterial().SetAsActive(m_pDeviceContext, m_pTexManager);
            m_LastDrawnMeshID = pMeshInst->GetMeshID();
        }

        if (mesh == nullptr || m_LastDrawnMeshID == INVALID_UID)
            continue;

        // Update world matrix.
        m_pDeviceContext->UpdateSubresource(m_PerObjectConstBuf.m_WorldMatrix, 0, nullptr, &pMeshInst->GetParentSceneNode()->GetWorldTransform(), 0, 0);
        m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_pDeviceContext->PSSetShaderResources(GPUTextureReg::DepthMap, 1, &m_pZBufferSRView);
        m_pDeviceContext->PSSetShaderResources(GPUTextureReg::ShadowMap, 1, &m_pShadowSRView);
        m_pDeviceContext->PSSetSamplers(GPUSamplerStateReg::Point, 1, &m_pPointSS);
        m_pDeviceContext->DrawIndexed(mesh->GetIndexBuffer().Count(), 0, 0);
    }

    // Unbind screen textures.
    ID3D11ShaderResourceView* pNullSRVs[] = { nullptr };
    m_pDeviceContext->PSSetShaderResources(GPUTextureReg::DepthMap, 1, pNullSRVs);
    m_pDeviceContext->PSSetShaderResources(GPUTextureReg::ShadowMap, 1, pNullSRVs);

    return true;
}

void Graphics::_HandleMessage(CBRefCountPtr<Message> &pMsg)
{
    if (pMsg->GetInstType() == Msg_SetMainCamera::ClassTypeSpecifier())
    {
        m_pMainCamera = MESSAGE_FROM_PTR(pMsg, Msg_SetMainCamera)->m_pCamera;
    }

}

bool Graphics::InitializePipeline()
{
    //////////////////////////////////////////////////////////////////////////
    // Create device, device context and swap chain.
    DXGI_SWAP_CHAIN_DESC swapDesc;
    ZeroMemory(&swapDesc, sizeof(swapDesc));
    swapDesc.BufferCount = 1;
    swapDesc.BufferDesc.Width = m_pWindow->GetWidth();
    swapDesc.BufferDesc.Height = m_pWindow->GetHeight();
    DXGI_RATIONAL refreshRate;
    refreshRate.Numerator = 60;
    refreshRate.Denominator = 1;
    swapDesc.BufferDesc.RefreshRate = refreshRate;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.OutputWindow = m_pWindow->GetWindowHandle();
    swapDesc.SampleDesc.Count = 8;
    swapDesc.SampleDesc.Quality = 0;
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapDesc.Windowed = !m_pWindow->GetIsFullScreen();

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };
    D3D_FEATURE_LEVEL featureLevel;

    ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_DEBUG,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        &swapDesc,
        &m_pSwapChain,
        &m_pDevice,
        &featureLevel,
        &m_pDeviceContext
    ));

    //UINT tempQuality;
    //m_pDevice->CheckMultisampleQualityLevels(swapDesc.BufferDesc.Format, 
    //                                         swapDesc.SampleDesc.Count, 
    //                                         &tempQuality);

    //////////////////////////////////////////////////////////////////////////
    // Create depth stencil buffers, views and states.

    // Depth stencil Buffers:
    D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
    ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
    depthStencilBufferDesc.ArraySize = 1;
    depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilBufferDesc.CPUAccessFlags = 0;
    depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilBufferDesc.Width = m_pWindow->GetWidth();
    depthStencilBufferDesc.Height = m_pWindow->GetHeight();
    depthStencilBufferDesc.MipLevels = 1;
    depthStencilBufferDesc.SampleDesc.Count = swapDesc.SampleDesc.Count;
    depthStencilBufferDesc.SampleDesc.Quality = swapDesc.SampleDesc.Quality;
    depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    ThrowIfFailed(m_pDevice->CreateTexture2D(&depthStencilBufferDesc, nullptr, &m_pDepthStencilBuffer));

    depthStencilBufferDesc.SampleDesc.Count = 1;
    depthStencilBufferDesc.SampleDesc.Quality = 0;
    ThrowIfFailed(m_pDevice->CreateTexture2D(&depthStencilBufferDesc, nullptr, &m_pDepthStencilBufferNoMS));

    depthStencilBufferDesc.Width = g_ShadowMapW;
    depthStencilBufferDesc.Height = g_ShadowMapH;
    ThrowIfFailed(m_pDevice->CreateTexture2D(&depthStencilBufferDesc, nullptr, &m_pDepthStencilBufferShadow));


    // Depth stencil Views:
    ThrowIfFailed(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, nullptr, &m_pDepthStencilView));
    ThrowIfFailed(m_pDevice->CreateDepthStencilView(m_pDepthStencilBufferNoMS, nullptr, &m_pDepthStencilViewNoMS));
    ThrowIfFailed(m_pDevice->CreateDepthStencilView(m_pDepthStencilBufferShadow, nullptr, &m_pDepthStencilViewShadow));

    // Depth stencil States:
    // Create depth/stencil state.
    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
    ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilStateDesc.DepthEnable = TRUE;
    depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // mess with this for transparency
    depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilStateDesc.StencilEnable = FALSE;
    ThrowIfFailed(m_pDevice->CreateDepthStencilState(&depthStencilStateDesc, &m_pDepthStencilState));
    ThrowIfFailed(m_pDevice->CreateDepthStencilState(&depthStencilStateDesc, &m_pDepthStencilStateNoMS));
    ThrowIfFailed(m_pDevice->CreateDepthStencilState(&depthStencilStateDesc, &m_pDepthStencilStateShadow));


    //////////////////////////////////////////////////////////////////////////
    // Create render targets.

    // Swap chain back buffer:
    ID3D11Texture2D *pBackBuffer;
    ThrowIfFailed(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));
    ThrowIfFailed(m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView));

    // Depth map:
    D3D11_TEXTURE2D_DESC texDesc;
    ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
    texDesc.ArraySize = 1;
    texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.Format = DXGI_FORMAT_R32_FLOAT;
    texDesc.Width = m_pWindow->GetWidth();
    texDesc.Height = m_pWindow->GetHeight();
    texDesc.MipLevels = 1;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    ThrowIfFailed(m_pDevice->CreateTexture2D(&texDesc, nullptr, &m_pZBuffer));

    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    renderTargetViewDesc.Format = texDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;
    ThrowIfFailed(m_pDevice->CreateRenderTargetView(m_pZBuffer, &renderTargetViewDesc, &m_pZBufferRTView));

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    ThrowIfFailed(m_pDevice->CreateShaderResourceView(m_pZBuffer, &srvDesc, &m_pZBufferSRView));

    // Shadow Map.
    texDesc.Width = g_ShadowMapW;
    texDesc.Height = g_ShadowMapH;
    ThrowIfFailed(m_pDevice->CreateTexture2D(&texDesc, nullptr, &m_pShadowMap));
    ThrowIfFailed(m_pDevice->CreateRenderTargetView(m_pShadowMap, &renderTargetViewDesc, &m_pShadowRTView));
    ThrowIfFailed(m_pDevice->CreateShaderResourceView(m_pShadowMap, &srvDesc, &m_pShadowSRView));

    //////////////////////////////////////////////////////////////////////////
    // Rasterizer.

    // Create rasterizer state.
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
    rasterizerDesc.AntialiasedLineEnable = FALSE;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0.0f;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.FrontCounterClockwise = FALSE;
    rasterizerDesc.MultisampleEnable = FALSE;
    rasterizerDesc.ScissorEnable = FALSE;
    rasterizerDesc.SlopeScaledDepthBias = 0.0f;
    ID3D11RasterizerState* pRasterizerState;
    ThrowIfFailed( m_pDevice->CreateRasterizerState(&rasterizerDesc, &pRasterizerState));
    m_pDeviceContext->RSSetState(pRasterizerState);

    // Create viewport.
    ZeroMemory(&m_MainViewport, sizeof(D3D11_VIEWPORT));
    m_MainViewport.TopLeftX = 0;
    m_MainViewport.TopLeftY = 0;
    m_MainViewport.Width = static_cast<FLOAT>(m_pWindow->GetWidth());
    m_MainViewport.Height = static_cast<FLOAT>(m_pWindow->GetHeight());
    m_MainViewport.MinDepth = 0.0f;
    m_MainViewport.MaxDepth = 1.0f;

    ZeroMemory(&m_ShadowViewport, sizeof(D3D11_VIEWPORT));
    m_ShadowViewport.TopLeftX = 0;
    m_ShadowViewport.TopLeftY = 0;
    m_ShadowViewport.Width = static_cast<FLOAT>(g_ShadowMapW);
    m_ShadowViewport.Height = static_cast<FLOAT>(g_ShadowMapH);
    m_ShadowViewport.MinDepth = 0.0f;
    m_ShadowViewport.MaxDepth = 1.0f;

    //////////////////////////////////////////////////////////////////////////
    // Other.

    // Create and set some constant buffers.
    m_PerFrameConstBuf.CreateBuffers(m_pDevice);
    m_PerObjectConstBuf.CreateBuffers(m_pDevice);
    m_PerFrameConstBuf.SetBuffers(m_pDeviceContext);
    m_PerObjectConstBuf.SetBuffers(m_pDeviceContext);

    // Depth shaders
    ID3D10Blob *VS = nullptr;
    ID3D10Blob *PS = nullptr;
    D3DReadFileToBlob(CBPath::GetShaderPath("depthOnly_vs.cso"), &VS);
    D3DReadFileToBlob(CBPath::GetShaderPath("depthOnly_ps.cso"), &PS);
    ThrowIfFailed(m_pDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pDepthOnlyVS));
    ThrowIfFailed(m_pDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pDepthOnlyPS));
    // Create input layout.
    ThrowIfFailed(m_pDevice->CreateInputLayout(Vertex::InputDesc, 4, VS->GetBufferPointer(), VS->GetBufferSize(), &m_pDepthInputLayout));
    VS->Release();
    PS->Release();
    VS = nullptr;
    PS = nullptr;
    ID3D10Blob *shadowVSBlob = nullptr;
    D3DReadFileToBlob(CBPath::GetShaderPath("shadowMap_vs.cso"), &shadowVSBlob);
    ThrowIfFailed(m_pDevice->CreateVertexShader(shadowVSBlob->GetBufferPointer(), shadowVSBlob->GetBufferSize(), NULL, &m_pShadowMapVS));

    // Create sampler state.
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    m_pPointSS = nullptr;
    ThrowIfFailed(m_pDevice->CreateSamplerState(&sampDesc, &m_pPointSS));

    return true;
}

