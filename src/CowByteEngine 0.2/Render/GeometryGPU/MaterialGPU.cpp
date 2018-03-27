#include <d3dcompiler.h>
#include "MaterialGPU.h"
#include "GPURegisterLayout.h"
#include "../../Utils/CBDebug.h"
#include "../../Utils/CBPath.h"
#include "../GeometryCPU/Vertex.h"

using namespace DirectX;

MaterialGPU::MaterialGPU() :
    m_Textures(4),
    m_pConstBuf(),
    m_pShaderVertex(nullptr),
    m_pShaderPixel(nullptr),
    m_pInputLayout(nullptr)
{
}


MaterialGPU::~MaterialGPU()
{
}

bool MaterialGPU::LoadFromMaterialCPU(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext, const MaterialCPU &matcpu)
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
        return false;
    }
    pDeviceContext->UpdateSubresource(m_pConstBuf, 0, nullptr, &matcpu.GetConstBufferCPU(), 0, 0);

    // Create textures. 
    // TODO: have a texture manager so we share textures.
    m_Textures.Push_back(TextureGPU());
    m_Textures.Back()->LoadFromFile(pDevice, matcpu.GetMapName(TexMapType::Albedo).Peek());
    m_Textures.Push_back(TextureGPU());
    m_Textures.Back()->LoadFromFile(pDevice, matcpu.GetMapName(TexMapType::Normal).Peek());
    m_Textures.Push_back(TextureGPU());
    m_Textures.Back()->LoadFromFile(pDevice, matcpu.GetMapName(TexMapType::Specular).Peek());

    // Create shaders.
    ID3D10Blob *VS = nullptr;
    ID3D10Blob *PS = nullptr;
    D3DReadFileToBlob(CBPath::GetShaderPath(matcpu.GetVSName().Peek()), &VS);
    D3DReadFileToBlob(CBPath::GetShaderPath(matcpu.GetPSName().Peek()), &PS);
    if (VS == nullptr || PS == nullptr)
    {
        DbgERROR("Failed reading shader files.");
        return false;
    }
    if (!ResultNotFailed(pDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pShaderVertex)))
    {
        DbgERROR("Filed creating vertex shader!");
        return false;
    }
    if (!ResultNotFailed(pDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pShaderPixel)))
    {
        DbgERROR("Filed creating pixel shader!");
        return false;
    }

    // Create input layout.
    if (!ResultNotFailed(pDevice->CreateInputLayout(Vertex::InputDesc, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout)))
    {
        DbgERROR("Filed creating input layout!");
        return false;
    }

    return true;
}

void MaterialGPU::SetAsActive(ID3D11DeviceContext *pDeviceContext) const
{
    // Set constant buffer.
    pDeviceContext->VSSetConstantBuffers(GPUConstants::PerMat, 1, &m_pConstBuf);

    // Set shader
    pDeviceContext->VSSetShader(m_pShaderVertex, 0, 0);
    pDeviceContext->PSSetShader(m_pShaderPixel, 0, 0);

    // Set textures
    for (size_t i = 0; i < m_Textures.Size(); ++i)
    {
        pDeviceContext->PSSetSamplers(i, 1, &m_Textures.peekat(i).GetSamplerState());
        pDeviceContext->PSSetShaderResources(i, 1, &m_Textures.peekat(i).GetShaderResourceView());
    }

    // Set input layout.
    pDeviceContext->IASetInputLayout(m_pInputLayout);
}
