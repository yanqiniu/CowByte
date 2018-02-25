#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <windows.h>
#include <windowsx.h>

#include <d3d12.h>
#pragma comment (lib, "d3d12.lib")
#include "d3dx12.h"
#include <wrl/client.h>
#include <dxgi1_4.h>
#include <DirectXMath.h>

#include "System.h"
#include "Window.h"

using namespace Microsoft::WRL;
using namespace DirectX;

struct GraphicsData : SystemData
{
    explicit GraphicsData();

    bool m_UseWarpDevice;
    DXGI_FORMAT m_BufferFormat;


};

// Temporary vertex struct to get me pass the tutorial.
struct Vertex
{
    XMFLOAT3 position;
    XMFLOAT4 color;
};

class Graphics : public System
{
public:
    Graphics(const GraphicsData& data, Window* window);
    virtual ~Graphics();
    virtual bool Initialize();
    virtual bool Update(Context& context);
    virtual bool ShutDown();

private:
    void LoadPipeline();
    void LoadAssets(); // May be deprecated later.
    void PopulateCommandList();
    void OnRender();
    void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter);

    static const UINT SwapChainBufferCount = 2;
    // Might not want to use all these smart pointers in the 
    // future but rather have more defined memory behavior,
    // but let's not worry too much now.
    ComPtr<ID3D12Device> m_Device;
    ComPtr<ID3D12CommandQueue> m_CommandQueue;
    ComPtr<IDXGISwapChain3> m_SwapChain;
    ComPtr<ID3D12DescriptorHeap> m_RtvHeap;
    ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
    ComPtr<ID3D12Resource> m_RenderTargets[SwapChainBufferCount];
    ComPtr<ID3D12RootSignature> m_RootSignature;
    ComPtr<ID3D12GraphicsCommandList> m_GraphicsCommandList;
    ComPtr<ID3D12PipelineState> m_PipelineState;
    UINT m_rtvDescriptorSize;

    ComPtr<ID3D12Resource> m_VertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;

    UINT m_FrameIndex;
    float m_AspectRatio;

#pragma region Pipeline configurations
    bool m_UseWarpDevice;
    int m_BufferWidth;
    int m_BufferHeight;
    DXGI_FORMAT m_BufferFormat;
#pragma endregion
    Window* m_pWindow;


};

#endif