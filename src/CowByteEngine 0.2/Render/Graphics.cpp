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
    m_pRasterizerState(nullptr),
    m_pZBufferSS(nullptr),
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
        //!m_pMeshManager->CPULoadMesh("cow.mesha") ||
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
        DbgWARNING("No main camera set (send the message)!");
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
    m_pLightManager->UpdateLightsGPU(m_pDeviceContext);
    PassDepthOnly();
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
    FLOAT color = 1.0f;
    m_pDeviceContext->OMSetRenderTargets(1, &m_pZBufferView, m_pDepthStencilView);
    m_pDeviceContext->ClearRenderTargetView(m_pZBufferView, &color);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);

    m_pDeviceContext->VSSetShader(m_pDepthOnlyVS, nullptr, 0);
    m_pDeviceContext->PSSetShader(m_pDepthOnlyPS, nullptr, 0);
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
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);

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
        m_pDeviceContext->PSSetShaderResources(GPUTextureReg::DepthMap, 1, &m_pZBufferRscView);
        m_pDeviceContext->PSSetSamplers(GPUTextureReg::DepthMap, 1, &m_pZBufferSS);
        m_pDeviceContext->DrawIndexed(mesh->GetIndexBuffer().Count(), 0, 0);
    }

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
    DXGI_SWAP_CHAIN_DESC swapDesc;

    ZeroMemory(&swapDesc, sizeof(swapDesc));
    swapDesc.BufferCount = NumOfBuffers;
    swapDesc.BufferDesc.Width = m_pWindow->GetWidth();
    swapDesc.BufferDesc.Height = m_pWindow->GetHeight();
    DXGI_RATIONAL refreshRate;
    refreshRate.Numerator = 60;
    refreshRate.Denominator = 1;
    swapDesc.BufferDesc.RefreshRate = refreshRate;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.OutputWindow = m_pWindow->GetWindowHandle();
    swapDesc.SampleDesc.Count = 1;
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
        0,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        &swapDesc,
        &m_pSwapChain,
        &m_pDevice,
        &featureLevel,
        &m_pDeviceContext
    ));



    // Create a depth stencil buffer and view.
    D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
    ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
    depthStencilBufferDesc.ArraySize = 1;
    depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilBufferDesc.CPUAccessFlags = 0;
    depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilBufferDesc.Width = m_pWindow->GetWidth();
    depthStencilBufferDesc.Height = m_pWindow->GetHeight();
    depthStencilBufferDesc.MipLevels = 1;
    depthStencilBufferDesc.SampleDesc.Count = 1;
    depthStencilBufferDesc.SampleDesc.Quality = 0;
    depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    ThrowIfFailed(m_pDevice->CreateTexture2D(&depthStencilBufferDesc, nullptr, &m_pDepthStencilBuffer));
    ThrowIfFailed(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, nullptr, &m_pDepthStencilView));

    // Create depth/stencil state.
    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
    ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilStateDesc.DepthEnable = TRUE;
    depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // mess with this for transparency
    depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilStateDesc.StencilEnable = FALSE;
    ThrowIfFailed(m_pDevice->CreateDepthStencilState(&depthStencilStateDesc, &m_pDepthStencilState));


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
    ThrowIfFailed( m_pDevice->CreateRasterizerState(&rasterizerDesc, &m_pRasterizerState));

    // Create viewport.
    ZeroMemory(&m_Viewport, sizeof(D3D11_VIEWPORT));
    m_Viewport.TopLeftX = 0;
    m_Viewport.TopLeftY = 0;
    m_Viewport.Width = static_cast<FLOAT>(m_pWindow->GetWidth());
    m_Viewport.Height = static_cast<FLOAT>(m_pWindow->GetHeight());
    m_Viewport.MinDepth = 0.0f;
    m_Viewport.MaxDepth = 1.0f;

    // Create and set some constant buffers.
    m_PerFrameConstBuf.CreateBuffers(m_pDevice);
    m_PerObjectConstBuf.CreateBuffers(m_pDevice);
    m_PerFrameConstBuf.SetBuffers(m_pDeviceContext);
    m_PerObjectConstBuf.SetBuffers(m_pDeviceContext);

    // Set rasterizer stage.
    m_pDeviceContext->RSSetState(m_pRasterizerState);
    m_pDeviceContext->RSSetViewports(1, &m_Viewport);

    // Output merger stage.
    // Create render targets.
    ID3D11Texture2D *pBackBuffer;
    ThrowIfFailed(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));
    ThrowIfFailed(m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView));
    //pBackBuffer->Release();

    // Z Buffer:
    D3D11_TEXTURE2D_DESC zbufDesc;
    ZeroMemory(&zbufDesc, sizeof(D3D11_TEXTURE2D_DESC));
    zbufDesc.ArraySize = 1;
    zbufDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    zbufDesc.CPUAccessFlags = 0;
    zbufDesc.Format = DXGI_FORMAT_R32_FLOAT;
    zbufDesc.Width = m_pWindow->GetWidth();
    zbufDesc.Height = m_pWindow->GetHeight();
    zbufDesc.MipLevels = 1;
    zbufDesc.SampleDesc.Count = 1;
    zbufDesc.SampleDesc.Quality = 0;
    zbufDesc.Usage = D3D11_USAGE_DEFAULT;
    if (!ResultNotFailed(m_pDevice->CreateTexture2D(&zbufDesc, nullptr, &m_pZBuffer)))
    {
        return false;
    }
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    renderTargetViewDesc.Format = zbufDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;
    if (!ResultNotFailed(m_pDevice->CreateRenderTargetView(m_pZBuffer, &renderTargetViewDesc, &m_pZBufferView)))
    {
        return false;
    }
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = zbufDesc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    if (!ResultNotFailed(m_pDevice->CreateShaderResourceView(m_pZBuffer, &srvDesc, &m_pZBufferRscView)))
    {
        return false;
    }

    //m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    m_pZBufferSS = nullptr;
    if (!ResultNotFailed(m_pDevice->CreateSamplerState(&sampDesc, &m_pZBufferSS)))
    {
        return false;
    }



    m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);


    // Initialize z-buffer related:
    ID3D10Blob *VS = nullptr;
    ID3D10Blob *PS = nullptr;
    D3DReadFileToBlob(CBPath::GetShaderPath("depthOnly_vs.cso"), &VS);
    D3DReadFileToBlob(CBPath::GetShaderPath("depthOnly_ps.cso"), &PS);
    if (!ResultNotFailed(m_pDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pDepthOnlyVS)))
    {
        return false;
    }
    if (!ResultNotFailed(m_pDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pDepthOnlyPS)))
    {
        return false;
    }

    VS->Release();
    PS->Release();
    VS = nullptr;
    PS = nullptr;




    return true;
}

