#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <windows.h>
#include <windowsx.h>

#include <d3d12.h>
#pragma comment (lib, "d3d12.lib")
#include "d3dx12.h"

#include "System.h"
#include "Window.h"

using namespace Microsoft::WRL;

struct GraphicsData : SystemData
{
    explicit GraphicsData();

    bool m_UseWarpDevice;
    DXGI_FORMAT m_BufferFormat;
};

class Graphics : public System
{
public:
    Graphics(const GraphicsData& data, Window* window);
    virtual ~Graphics();

private:
    void LoadPipeline();
    void LoadAssets(); // May be deprecated later.
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
    UINT m_rtvDescriptorSize;


    UINT m_FrameIndex;

#pragma region Pipeline configurations
    bool m_UseWarpDevice;
    int m_BufferWidth;
    int m_BufferHeight;
    DXGI_FORMAT m_BufferFormat;
#pragma endregion
    Window* m_pWindow;


};

#endif