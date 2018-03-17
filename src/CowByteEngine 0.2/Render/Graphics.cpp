#include <exception>
#include <DirectXMath.h>

#include "Graphics.h"
#include "Window.h"
#include "../Utils/exceptions.h"
#include "Vertex.h"
#include "../Math/Vec3.h"
#include "../Utils/CBString.h"
#include "../Utils/CBPath.h"
#include "../Utils/CBDebug.h"
#include "../Utils/typedefs.h"
#include "../SceneGraph/Camera.h"
#include "../SceneGraph/SceneNode.h"
#include "MeshInstance.h"
#include "../Utils/CBFile.h"
#include "../Core/MessageBus.h"

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
    m_pRenderTargetView(nullptr),
    m_pDepthStencilView(nullptr),
    m_pVertexBuffer(nullptr),
    m_pIndexBuffer(nullptr),
    m_pConstantBuffers(),
    m_pDepthStencilBuffer(nullptr),
    m_pInputLayout(nullptr),
    m_pDepthStencilState(nullptr),
    m_pRasterizerState(nullptr),
    m_LastDrawnMeshID(INVALID_UID)
{

}

Graphics::~Graphics()
{
}

SceneNode *g_pCamNode;
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

    // Create mesh manager.
    m_pMeshManager = new MeshManager();
    m_pMeshManager->AttachTo_NonSceneNode_Parent(this);
    m_pMeshManager->CPULoadMesh("cube.mesha");


    // Camera setup.
    m_pMainCamera = new Camera((float)m_pWindow->GetWidth() / m_pWindow->GetHeight(),
        0.698131f, 0.01f, 1000.0f);
    g_pCamNode = SceneNode::CreateSceneNodeThenAttach(&SceneNode::RootNode);
    m_pMainCamera->AttachTo_SceneNode_Parent(g_pCamNode);
    g_pCamNode->Translate(Vec3(0, 0, -5.0f));

    m_pDeviceContext->UpdateSubresource(m_pConstantBuffers[ConstantBufferType::CBUFFER_APPLICATION], 0, nullptr, &m_pMainCamera->GetProjectionMatrix(), 0, 0);

    return true;
}

bool Graphics::Update(const GameContext& context)
{
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);

    g_pCamNode->UpdateWorldTransform();
    m_pMainCamera->UpdateWToCMatrix();
    m_pDeviceContext->UpdateSubresource(m_pConstantBuffers[ConstantBufferType::CBUFFER_FRAME], 0, nullptr, &m_pMainCamera->GetWToCMatrix(), 0, 0);

    for (int i = 0; i < m_pMeshManager->GetMeshInsts().Size(); ++i)
    {
        SetupSingleMeshInst(m_pMeshManager->GetMeshInsts().peekat(i));
        m_pDeviceContext->VSSetConstantBuffers(0, 3, m_pConstantBuffers);
        OnRender();

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

bool Graphics::OnRender()
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
    m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_pDeviceContext->DrawIndexed(36, 0, 0);

    return true;
}

void Graphics::_HandleMessage(CBRefCountPtr<Message> &pMsg)
{

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


    // Set render target.
    ID3D11Texture2D *pBackBuffer;
    ThrowIfFailed(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));
    ThrowIfFailed(m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView));
    pBackBuffer->Release();
    m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

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


    // Load and compile shaders. These will be put into a Mesh class.
    ID3D10Blob *VS, *PS;
    ID3D11VertexShader *pVS;
    ID3D11PixelShader *pPS;

    D3DReadFileToBlob(L"../x64/Debug/default_vs.cso", &VS);
    ThrowIfFailed(m_pDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS));

    D3DReadFileToBlob(L"../x64/Debug/default_ps.cso", &PS);
    ThrowIfFailed(m_pDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS));

    // set the shader objects
    m_pDeviceContext->VSSetShader(pVS, 0, 0);
    m_pDeviceContext->PSSetShader(pPS, 0, 0);


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

    // Create Vertex buffers.
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = sizeof(Vertex) * 666; // TODO: remove this magic number and use something "large enough"
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    ThrowIfFailed(m_pDevice->CreateBuffer(&bufferDesc, nullptr, &m_pVertexBuffer));

    // Create Index buffer.
    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
    indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    indexBufferDesc.ByteWidth = sizeof(WORD) * 6666;  // TODO: remove this magic number and use something "large enough"
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    ThrowIfFailed(m_pDevice->CreateBuffer(&indexBufferDesc, nullptr, &m_pIndexBuffer));


    // Create input layout.
    m_pDevice->CreateInputLayout(Vertex::InputDesc, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);
    m_pDeviceContext->IASetInputLayout(m_pInputLayout);

    // Set rasterizer stage.
    m_pDeviceContext->RSSetState(m_pRasterizerState);
    m_pDeviceContext->RSSetViewports(1, &m_Viewport);

    // Output merger stage.
    m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
    m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);


    return true;
}

bool Graphics::SetupSingleMeshInst(MeshInstance *meshInst)
{
    if (m_LastDrawnMeshID != meshInst->GetMeshID()) // Only refresh buffer when needed.
    {
        Mesh* mesh = m_pMeshManager->GetMeshPtr(meshInst->GetMeshID());

        // Update vertex buffer.
        D3D11_MAPPED_SUBRESOURCE mappedSubrcs; // information about buffer once mapped, including location of the buffer.
        ThrowIfFailed(m_pDeviceContext->Map(m_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubrcs));
        memcpy(mappedSubrcs.pData, &mesh->GetVertices()[0], mesh->GetNumVertices() * sizeof(Vertex));
        m_pDeviceContext->Unmap(m_pVertexBuffer, NULL);


        // Update index buffer.
        ZeroMemory(&mappedSubrcs, sizeof(D3D11_MAPPED_SUBRESOURCE));
        ThrowIfFailed(m_pDeviceContext->Map(m_pIndexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubrcs));
        memcpy(mappedSubrcs.pData, &mesh->GetIndices()[0], mesh->GetNumTriangles() * 3 * sizeof(WORD));
        m_pDeviceContext->Unmap(m_pIndexBuffer, NULL);

        m_LastDrawnMeshID = meshInst->GetMeshID();
    }


    // Update world matrix.
    m_pDeviceContext->UpdateSubresource(m_pConstantBuffers[ConstantBufferType::CBUFFER_OBJECT], 0, nullptr, &meshInst->GetParentSceneNode()->GetWorldTransform(), 0, 0);

    return true;
}

