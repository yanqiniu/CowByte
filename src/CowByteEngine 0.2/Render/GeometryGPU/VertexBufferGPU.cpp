#include "VertexBufferGPU.h"

using namespace DirectX;

VertexBufferGPU::VertexBufferGPU() :
    m_pVertexBuffer(nullptr)
{
}


VertexBufferGPU::~VertexBufferGPU()
{

}

// Create the vertex buffer and fill it with vertices
bool VertexBufferGPU::InitFromVertexVector(ID3D11Device *pD3DDevice, ID3D11DeviceContext *pDeviceContext, CBVector<Vertex> &vertices)
{
    // Create Vertex buffers.
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = sizeof(Vertex) * vertices.Size();
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    if (!ResultNotFailed(pD3DDevice->CreateBuffer(&bufferDesc, nullptr, &m_pVertexBuffer)))
    {
        Release();
        return false;
    }

    // Fill it with vertices.
    D3D11_MAPPED_SUBRESOURCE mappedSubrcs; 
    if (!ResultNotFailed(pDeviceContext->Map(m_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubrcs)))
    {
        Release();
        return false;
    }
    memcpy(mappedSubrcs.pData, &vertices[0], sizeof(Vertex) * vertices.Size());
    pDeviceContext->Unmap(m_pVertexBuffer, NULL);

    return true;
}

void VertexBufferGPU::Release()
{
    if (m_pVertexBuffer != nullptr)
    {
        m_pVertexBuffer->Release();
        m_pVertexBuffer = nullptr;
    }
}

void VertexBufferGPU::SetAsActive(ID3D11DeviceContext *pDeviceContext) const
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
}

