#include "IndexBufferGPU.h"
#include "../../Utils/CBDebug.h"

using namespace DirectX;

IndexBufferGPU::IndexBufferGPU():
    m_pIndexBuffer(nullptr),
    m_IndexCount(0)
{

}



IndexBufferGPU::~IndexBufferGPU()
{
}

bool IndexBufferGPU::InitFromWORDVector(ID3D11Device *pD3Device, ID3D11DeviceContext *pDeviceContext, CBVector<WORD> &indices)
{
    // Create Index buffer.
    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
    indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    indexBufferDesc.ByteWidth = sizeof(WORD) * indices.Size();
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    if (!ResultNotFailed(pD3Device->CreateBuffer(&indexBufferDesc, nullptr, &m_pIndexBuffer)))
    {
        Release();
        return false;
    }

    // Fill the buffer.
    D3D11_MAPPED_SUBRESOURCE mappedSubrcs; // information about buffer once mapped, including location of the buffer.
    ZeroMemory(&mappedSubrcs, sizeof(D3D11_MAPPED_SUBRESOURCE));
    if (!ResultNotFailed(pDeviceContext->Map(m_pIndexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubrcs)))
    {
        Release();
        return false;
    }
    memcpy(mappedSubrcs.pData, &indices[0], sizeof(WORD) * indices.Size());
    pDeviceContext->Unmap(m_pIndexBuffer, NULL);

    m_IndexCount = indices.Size();

    return true;
}

void IndexBufferGPU::Release()
{
    if (m_pIndexBuffer != nullptr)
    {
        m_pIndexBuffer->Release();
        m_pIndexBuffer = nullptr;
    }

}

void IndexBufferGPU::SetAsActive(ID3D11DeviceContext *pDeviceContext) const
{
    pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
}
