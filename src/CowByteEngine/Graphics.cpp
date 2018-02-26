#include <exception>

#include "Graphics.h"
#include "exceptions.h"

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
    m_Device(nullptr),
    m_DeviceContext(nullptr),
    m_SwapChain(nullptr)
{

}

Graphics::~Graphics()
{
}

bool Graphics::Initialize()
{
    if (m_pWindow == nullptr)
    {
        //Logger::Log("Graphics: Window pointer null!");
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
        &m_SwapChain,
        &m_Device,
        NULL,
        &m_DeviceContext
    ));


    // Set render target.
    ID3D11Texture2D *pBackBuffer;
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    m_Device->CreateRenderTargetView(pBackBuffer, NULL, &m_RenderTarget);
    pBackBuffer->Release();
    m_DeviceContext->OMSetRenderTargets(1, &m_RenderTarget, NULL);

    // Set viewport.
    ZeroMemory(&m_Viewport, sizeof(D3D11_VIEWPORT));
    m_Viewport.TopLeftX = 0;
    m_Viewport.TopLeftY = 0;
    m_Viewport.Width = m_pWindow->GetWidth();
    m_Viewport.Height = m_pWindow->GetHeight();
    m_DeviceContext->RSSetViewports(1, &m_Viewport);

    return true;
}

bool Graphics::Update(Context& context)
{
    m_DeviceContext->ClearRenderTargetView(m_RenderTarget, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

    m_SwapChain->Present(0, 0);
    return true;
}

bool Graphics::ShutDown()
{
    m_SwapChain->Release();
    m_Device->Release();
    m_DeviceContext->Release();
    m_RenderTarget->Release();
    return true;
}


