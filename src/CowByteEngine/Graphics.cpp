#include <exception>

#include "Graphics.h"
#include "exceptions.h"

GraphicsData::GraphicsData() :
    SystemData(SystemType::SYS_GRAPHICS),
    m_UseWarpDevice(false),
    m_BufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM)
{

}


Graphics::Graphics(const GraphicsData& data, Window* window) :
    System(data),
    m_Device(nullptr),
    m_UseWarpDevice(data.m_UseWarpDevice),
    m_BufferWidth(window->GetWidth()),
    m_BufferHeight(window->GetHeight()),
    m_BufferFormat(data.m_BufferFormat),
    m_pWindow(window),
    m_FrameIndex(0)
{

}

Graphics::~Graphics()
{
}

bool Graphics::Initialize()
{
    System::Initialize();
    
    LoadPipeline();

    return true;
}

bool Graphics::Update(Context& context)
{
    return true;
}

bool Graphics::ShutDown()
{
    return true;
}

void Graphics::LoadPipeline()
{
#if defined(_DEBUG)
    // Enable the D3D12 debug layer.
    {
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();
        }
        else
        {
            //Logger::Log("Failed to enable D3D12 debug interface.")
        }
    }
#endif

    ComPtr<IDXGIFactory4> factory;
    DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

    // Create devices
    if (m_UseWarpDevice) // Use WARP software rasterizer?
    {
        ComPtr<IDXGIAdapter> warpAdapter;
        DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&warpAdapter)));

        DX::ThrowIfFailed(D3D12CreateDevice(
            warpAdapter.Get(),
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&m_Device)
        ));
    }
    else
    {
        ComPtr<IDXGIAdapter1> hardwareAdapter;
        GetHardwareAdapter(factory.Get(), &hardwareAdapter);

        DX::ThrowIfFailed(D3D12CreateDevice(
            hardwareAdapter.Get(),
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&m_Device)
        ));
    }

    // Describe and create the command queue.
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    DX::ThrowIfFailed(m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue)));

    // Describe and create the swap chain.
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = SwapChainBufferCount;
    swapChainDesc.BufferDesc.Width = m_BufferWidth;
    swapChainDesc.BufferDesc.Height = m_BufferHeight;
    swapChainDesc.BufferDesc.Format = m_BufferFormat;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.OutputWindow = m_pWindow->GetWindowHandle();
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;

    // Create the swap chain
    ComPtr<IDXGISwapChain> swapChain;
    DX::ThrowIfFailed(factory->CreateSwapChain(
        m_CommandQueue.Get(),
        &swapChainDesc,
        &swapChain
    ));

    DX::ThrowIfFailed(swapChain.As(&m_SwapChain));
    DX::ThrowIfFailed(factory->MakeWindowAssociation(m_pWindow->GetWindowHandle(), DXGI_MWA_NO_ALT_ENTER));
    m_FrameIndex = m_SwapChain->GetCurrentBackBufferIndex();

    // Create descriptor heaps...OK WHAT IS THIS??
    {
        // Describe and create a render target view (RTV) descriptor heap.
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        DX::ThrowIfFailed(m_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RtvHeap)));

        m_rtvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    // Create frame resources.
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RtvHeap->GetCPUDescriptorHandleForHeapStart());

        // Create a RTV for each frame.
        for (UINT n = 0; n < SwapChainBufferCount; n++)
        {
            DX::ThrowIfFailed(m_SwapChain->GetBuffer(n, IID_PPV_ARGS(&m_RenderTargets[n])));
            m_Device->CreateRenderTargetView(m_RenderTargets[n].Get(), nullptr, rtvHandle);
            rtvHandle.Offset(1, m_rtvDescriptorSize);
        }
    }

    DX::ThrowIfFailed(m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator)));


}

// Copied from : https://msdn.microsoft.com/en-us/library/windows/desktop/dn770336(v=vs.85).aspx
void Graphics::GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter)
{
    *ppAdapter = nullptr;
    for (UINT adapterIndex = 0; ; ++adapterIndex)
    {
        IDXGIAdapter1* pAdapter = nullptr;
        if (DXGI_ERROR_NOT_FOUND == pFactory->EnumAdapters1(adapterIndex, &pAdapter))
        {
            // No more adapters to enumerate.
            break;
        }

        // Check to see if the adapter supports Direct3D 12, but don't create the
        // actual device yet.
        if (SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
        {
            *ppAdapter = pAdapter;
            return;
        }
        pAdapter->Release();
    }
}

