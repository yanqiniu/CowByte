#include <d3dcompiler.h>
#include "MaterialGPU.h"
#include "GPURegisterLayout.h"
#include "../../Utils/CBDebug.h"
#include "../../Utils/CBPath.h"
#include "../GeometryCPU/Vertex.h"

using namespace DirectX;

MaterialGPU::MaterialGPU() :
    m_TexIDs(4),
    m_pConstBuf(),
    m_pShaderVertex(nullptr),
    m_pShaderPixel(nullptr),
    m_pInputLayout(nullptr)
{
}


MaterialGPU::~MaterialGPU()
{
}

bool MaterialGPU::LoadFromMaterialCPU(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext, const MaterialCPU &matcpu, TextureManager *pTexMgrGPU)
{
    // Create and fill constant buffer.
    D3D11_BUFFER_DESC constantBufferDesc;
    ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.ByteWidth = sizeof(PerMatConstBufCPU);
    constantBufferDesc.CPUAccessFlags = 0;
    constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    if (!ResultNotFailed(pDevice->CreateBuffer(&constantBufferDesc, nullptr, &m_pConstBuf)))
    {
        DbgERROR("Failed creating per mat constant buffer!");
        Release();
        return false;
    }
    pDeviceContext->UpdateSubresource(m_pConstBuf, 0, nullptr, &matcpu.GetConstBufferCPU(), 0, 0);

    // Create textures. 
    for (size_t i = 0; i < matcpu.GetTextureCPUs().Size(); ++i)
    {
        UID tempID = pTexMgrGPU->LoadFromTextureCPU(pDevice, matcpu.GetTextureCPUs().peekat(i));
        if (tempID == INVALID_UID) return false;
        else m_TexIDs.Push_back(tempID);
    }

    // Create shaders.
    ID3D10Blob *VS = nullptr;
    ID3D10Blob *PS = nullptr;
    D3DReadFileToBlob(CBPath::GetShaderPath(matcpu.GetVSName().Peek()), &VS);
    D3DReadFileToBlob(CBPath::GetShaderPath(matcpu.GetPSName().Peek()), &PS);
    if (VS == nullptr || PS == nullptr)
    {
        DbgERROR("Failed reading shader files.");
        Release();
        return false;
    }
    if (!ResultNotFailed(pDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pShaderVertex)))
    {
        DbgERROR("Filed creating vertex shader!");
        Release();
        return false;
    }
    if (!ResultNotFailed(pDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pShaderPixel)))
    {
        DbgERROR("Filed creating pixel shader!");
        Release();
        return false;
    }

    // Create input layout.
    if (!ResultNotFailed(pDevice->CreateInputLayout(Vertex::InputDesc, 4, VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout)))
    {
        DbgERROR("Filed creating input layout!");
        Release();
        return false;
    }

    return true;
}

void MaterialGPU::SetAsActive(ID3D11DeviceContext *pDeviceContext, const TextureManager *pTexMgrGPU) const
{
    // Set constant buffer.
    pDeviceContext->VSSetConstantBuffers(GPUConstantsReg_VS::PerMat, 1, &m_pConstBuf);

    // Set shader
    pDeviceContext->VSSetShader(m_pShaderVertex, 0, 0);
    pDeviceContext->PSSetShader(m_pShaderPixel, 0, 0);

    // Set textures
    for (size_t i = 0; i < m_TexIDs.Size(); ++i)
    {
        const TextureGPU* pTexGPU = pTexMgrGPU->PeekTexture(m_TexIDs.peekat(i));
        pDeviceContext->PSSetSamplers(pTexGPU->GetReg(), 1, &pTexGPU->GetSamplerState());
        pDeviceContext->PSSetShaderResources(pTexGPU->GetReg(), 1, &pTexGPU->GetShaderResourceView());
    }

    // Set input layout.
    pDeviceContext->IASetInputLayout(m_pInputLayout);
}

void MaterialGPU::Release()
{   // Set textures
    //for (size_t i = 0; i < m_Textures.Size(); ++i)
    //{
    //    //m_Textures.at(i).Release();
    //}

    if (m_pShaderVertex != nullptr)
    {
        m_pShaderVertex->Release();
        m_pShaderVertex = nullptr;
    }
    if (m_pShaderPixel != nullptr)
    {
        m_pShaderPixel->Release();
        m_pShaderPixel = nullptr;
    }
    if (m_pInputLayout != nullptr)
    {
        m_pInputLayout->Release();
        m_pInputLayout = nullptr;
    }
    if (m_pConstBuf != nullptr)
    {
        m_pConstBuf->Release();
        m_pConstBuf = nullptr;
    }
}
