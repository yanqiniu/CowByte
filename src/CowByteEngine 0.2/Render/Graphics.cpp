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
    m_pDevice(nullptr),
    m_pDeviceContext(nullptr),
    m_pSwapChain(nullptr),
    m_pMeshManager(nullptr),
    m_pTexManager(nullptr),
    m_pRenderTargetView(nullptr),
    m_pDepthStencilView(nullptr),
    m_pConstantBuffers(),
    m_pDepthStencilBuffer(nullptr),
    m_pDepthStencilState(nullptr),
    m_pRasterizerState(nullptr),
    m_pSamplerState(nullptr),
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
        !m_pMeshManager->CPULoadMesh("cow.mesha")
        )
    {
        return false;
    }

    m_pTexManager = new TextureManager();
    if (!m_pMeshManager->GPULoadMeshes(m_pDevice, m_pDeviceContext, m_pTexManager))
        return false;
    return true;
}

bool Graphics::Update(const GameContext& context)
{
    if (m_pMainCamera == nullptr)
    {
        DbgWARNING("No main camera set (send the message)!");
        return false;
    }

    FLOAT color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);

    m_pMainCamera->UpdateWToCMatrix();
    m_pDeviceContext->UpdateSubresource(m_pConstantBuffers[ConstantBufferType::CBUFFER_FRAME], 0, nullptr, &m_pMainCamera->GetWToCMatrix(), 0, 0);
    m_pDeviceContext->VSSetConstantBuffers(GPUConstants::PerFrame, 1, &m_pConstantBuffers[ConstantBufferType::CBUFFER_FRAME]);

    for (int i = 0; i < m_pMeshManager->GetMeshInsts().Size(); ++i)
    {
        DrawSingleMeshInst(m_pMeshManager->GetMeshInsts().peekat(i));
    }

    m_pSwapChain->Present(0, 0);
    return true;
}

bool Graphics::ShutDown()
{
    m_pSwapChain->Release();
    m_pDevice->Release();
    m_pDeviceContext->Release();
    m_pRenderTargetView->Release();
    return true;
}

bool Graphics::OnRender(UINT numIndices)
{
    //UINT stride = sizeof(Vertex);
    //UINT offset = 0;
    //m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
    //m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    //
    //m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //m_pDeviceContext->DrawIndexed(numIndices, 0, 0);

    return true;
}

bool Graphics::DrawSingleMeshInst(const MeshInstance* pMeshInst)
{
    Mesh* mesh = m_pMeshManager->GetMeshPtr(pMeshInst->GetMeshID());;
    if (m_LastDrawnMeshID != pMeshInst->GetMeshID() && mesh->IsLoaded())
    {
        mesh->GetVertexBuffer().SetAsActive(m_pDeviceContext);
        mesh->GetIndexBuffer().SetAsActive(m_pDeviceContext);
        mesh->GetMaterial().SetAsActive(m_pDeviceContext, m_pTexManager);
        m_LastDrawnMeshID = pMeshInst->GetMeshID();
    }

    if (mesh == nullptr || m_LastDrawnMeshID == INVALID_UID)
        return false;

    // Update world matrix.
    m_pDeviceContext->UpdateSubresource(m_pConstantBuffers[ConstantBufferType::CBUFFER_OBJECT], 0, nullptr, &pMeshInst->GetParentSceneNode()->GetWorldTransform(), 0, 0);
    m_pDeviceContext->VSSetConstantBuffers(GPUConstants::PerObject, 1, &m_pConstantBuffers[ConstantBufferType::CBUFFER_OBJECT]);
    
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pDeviceContext->DrawIndexed(mesh->GetIndexBuffer().Count(), 0, 0);

    return true;
}

void Graphics::_HandleMessage(CBRefCountPtr<Message> &pMsg)
{
    if (pMsg->GetInstType() == Msg_SetMainCamera::ClassTypeSpecifier())
    {
        m_pMainCamera = MESSAGE_FROM_PTR(pMsg, Msg_SetMainCamera)->m_pCamera;
        m_pDeviceContext->UpdateSubresource(m_pConstantBuffers[ConstantBufferType::CBUFFER_APPLICATION], 0, nullptr, &m_pMainCamera->GetProjectionMatrix(), 0, 0);
        m_pDeviceContext->VSSetConstantBuffers(GPUConstants::PerApplication, 1, &m_pConstantBuffers[ConstantBufferType::CBUFFER_APPLICATION]);
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
    swapDesc.SampleDesc.Count = 4;
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
    depthStencilBufferDesc.SampleDesc.Count = 4;
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

    // Create the constant buffers for the variables defined in the vertex shader.
    D3D11_BUFFER_DESC constantBufferDesc;
    ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.ByteWidth = sizeof(Matrix4x4);
    constantBufferDesc.CPUAccessFlags = 0;
    constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    ThrowIfFailed(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &m_pConstantBuffers[ConstantBufferType::CBUFFER_APPLICATION]));
    ThrowIfFailed(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &m_pConstantBuffers[ConstantBufferType::CBUFFER_FRAME]));
    ThrowIfFailed(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &m_pConstantBuffers[ConstantBufferType::CBUFFER_OBJECT]));
    ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

    // Set rasterizer stage.
    m_pDeviceContext->RSSetState(m_pRasterizerState);
    m_pDeviceContext->RSSetViewports(1, &m_Viewport);

    // Output merger stage.
    // Set render target.
    ID3D11Texture2D *pBackBuffer;
    ThrowIfFailed(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));
    ThrowIfFailed(m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView));
    pBackBuffer->Release();
    m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
    m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);


    return true;
}

