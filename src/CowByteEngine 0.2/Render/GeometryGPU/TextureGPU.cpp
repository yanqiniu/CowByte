#include "TextureGPU.h"
#include "../DDSTextureLoader/DDSTextureLoader.h"
#include "../../Utils/CBPath.h"
#include "../../Utils/CBDebug.h"
#include "../../Utils/CBStringOps.h"

using namespace DirectX;

TextureGPU::TextureGPU() :
    m_pSamplerState(nullptr),
    m_pShaderRscView(nullptr),
    m_UID(INVALID_UID)
{
}


TextureGPU::~TextureGPU()
{
}

bool TextureGPU::LoadFromTextureCPU(ID3D11Device *pDevice, const TextureCPU &texCPU)
{
    // Load the texture.
    Filepath texturePath;
    CBPath::GenerateAssetPath(texturePath, "textures", texCPU.GetFilename().Peek());
    const wchar_t *newPath = CBStringOps::CharToWChar(texturePath.Get());
    if (!ResultNotFailed(CreateDDSTextureFromFile(pDevice, newPath, nullptr, &m_pShaderRscView)))
    {
        Release();
        return false;
    }

    // Create sampler state, 
    // TODO: currently the same sampler state for all textures.
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    m_pSamplerState = nullptr;
    if (!ResultNotFailed(pDevice->CreateSamplerState(&sampDesc, &m_pSamplerState)))
    {
        Release();
        return false;
    }

    static UID uidCounter = 0;
    m_UID = uidCounter++;
    m_TexFileName.Set(texCPU.GetFilename());
    switch (texCPU.GetType())
    {
    case Albedo:   m_RegSlot = GPUTextureReg::AlbedoMap; break;
    case Normal:   m_RegSlot = GPUTextureReg::NormalMap; break;
    case Specular: m_RegSlot = GPUTextureReg::SpecularMap; break;
    }
    return true;
}


void TextureGPU::Release()
{
    if (m_pSamplerState != nullptr)
    {
        m_pSamplerState->Release();
        m_pSamplerState = nullptr;
    }
    if (m_pShaderRscView != nullptr)
    {
        m_pShaderRscView->Release();
        m_pShaderRscView = nullptr;
    }
}
