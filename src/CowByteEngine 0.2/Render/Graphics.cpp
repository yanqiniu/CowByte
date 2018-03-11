#include <exception>

#include "Graphics.h"
#include "Window.h"
#include "../Utils/exceptions.h"
#include "Vertex.h"
#include "../Math/Vec3.h"
#include "../Utils/CBString.h"
#include "../Utils/CBPath.h"
#include "../Utils/CBDebug.h"


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
    m_pMeshManager(nullptr)
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

    DXGI_SWAP_CHAIN_DESC swapDesc;
    ZeroMemory(&swapDesc, sizeof(swapDesc));
    swapDesc.BufferCount = NumOfBuffers;
    swapDesc.BufferDesc.Width = 640; // TODO: remove hard coding.
    swapDesc.BufferDesc.Height = 480; // TODO: remove hard coding.
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.OutputWindow = m_pWindow->GetWindowHandle();
    swapDesc.SampleDesc.Count = 4;
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapDesc.Windowed = !m_pWindow->GetIsFullScreen();

    ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        NULL,
        NULL,
        NULL,
        D3D11_SDK_VERSION,
        &swapDesc,
        &m_pSwapChain,
        &m_pDevice,
        NULL,
        &m_pDeviceContext
    ));


    // Set render target.
    ID3D11Texture2D *pBackBuffer;
    ThrowIfFailed(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));
    ThrowIfFailed(m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTarget));
    pBackBuffer->Release();
    m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTarget, NULL);

    // Set viewport.
    ZeroMemory(&m_Viewport, sizeof(D3D11_VIEWPORT));
    m_Viewport.TopLeftX = 0;
    m_Viewport.TopLeftY = 0;
    m_Viewport.Width = m_pWindow->GetWidth();
    m_Viewport.Height = m_pWindow->GetHeight();
    m_pDeviceContext->RSSetViewports(1, &m_Viewport);

    // Load and compile shaders. These will be put into a Mesh class.
    ID3D10Blob *VS, *PS;
    ID3D11VertexShader *pVS;
    ID3D11PixelShader *pPS;
    CBString<256> shaderPath;
    Path::GenerateAssetPath(shaderPath, "shaders", "default.shader");
    D3DX11CompileFromFile(shaderPath.Get(), 0, 0, "VShader", "vs_5_0", 0, 0, 0, &VS, 0, 0);
    D3DX11CompileFromFile(shaderPath.Get(), 0, 0, "PShader", "ps_5_0", 0, 0, 0, &PS, 0, 0);

    // encapsulate both shaders into shader objects
    m_pDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
    m_pDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

    // set the shader objects
    m_pDeviceContext->VSSetShader(pVS, 0, 0);
    m_pDeviceContext->PSSetShader(pPS, 0, 0);


    // Test vertices for drawing a triangle.
    static const int NumVerts = 3;
    Vertex testVerts[NumVerts] =
    {
        Vertex(0.0f,    0.5f, 0.0f, 1.0f, 0.0f, 0.0f),
        Vertex(0.45f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f),
        Vertex(-0.45f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f)
    };

    // Create vertex buffer.
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = sizeof(Vertex) * NumVerts;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    ThrowIfFailed(m_pDevice->CreateBuffer(&bufferDesc, NULL, &m_pVertexBuffer));

    // Fill the vertex buffer.
    D3D11_MAPPED_SUBRESOURCE mappedSubrcs; // information about buffer once mapped, including location of the buffer.
    ThrowIfFailed(m_pDeviceContext->Map(m_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubrcs));
    memcpy(mappedSubrcs.pData, testVerts, sizeof(testVerts));
    m_pDeviceContext->Unmap(m_pVertexBuffer, NULL);


    // Create input layout.
    m_pDevice->CreateInputLayout(Vertex::InputDesc, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);
    m_pDeviceContext->IASetInputLayout(m_pInputLayout);


    // TEST: Create MeshManager and load it up.
    m_pMeshManager = new MeshManager();
    m_pMeshManager->CPULoadMesh("cube.mesha");

    return true;
}

bool Graphics::Update(GameContext& context)
{
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTarget, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
    OnRender();
    m_pSwapChain->Present(0, 0);
    return true;
}

bool Graphics::ShutDown()
{
    m_pSwapChain->Release();
    m_pDevice->Release();
    m_pDeviceContext->Release();
    m_pRenderTarget->Release();
    return true;
}

bool Graphics::OnRender()
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
    
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_pDeviceContext->Draw(3,             // the number of vertices to be drawn
                           0);            // the first vertex to be drawn



    return true;
}

