#include "TextureGPU.h"
#include "../DDSTextureLoader/DDSTextureLoader.h"
#include "../../Utils/CBPath.h"
#include "../../Utils/CBDebug.h"
#include "../../Utils/CBStringOps.h"

using namespace DirectX;

TextureGPU::TextureGPU() :
    m_pSamplerState(nullptr),
    m_pShaderRscView(nullptr)
{
}


TextureGPU::~TextureGPU()
{
}

bool TextureGPU::LoadFromFile(ID3D11Device *pDevice, const char *filename)
{
    // Load the texture.
    Filepath texturePath;
    CBPath::GenerateAssetPath(texturePath, "textures", filename);
    const wchar_t *newPath = CBStringOps::CharToWChar(texturePath.Get());
    if(!ResultNotFailed(CreateDDSTextureFromFile(pDevice, newPath, nullptr, &m_pShaderRscView)))
    {
        return false;
    }

    // Create sampler state, currently the same for all textures.
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
        return false;
    }

    return true;
}

void TextureGPU::Release()
{
    m_pSamplerState->Release();
    m_pShaderRscView->Release();
    m_pSamplerState = nullptr;
    m_pShaderRscView = nullptr;
}
