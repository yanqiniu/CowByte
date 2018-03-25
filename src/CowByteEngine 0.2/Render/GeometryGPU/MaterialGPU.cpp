#include "MaterialGPU.h"



MaterialGPU::MaterialGPU()
{
}


MaterialGPU::~MaterialGPU()
{
}

void MaterialGPU::SetAsActive(ID3D11DeviceContext *pDeviceContext)
{
    for (size_t i = 0; i < m_Textures.Size(); ++i)
    {
        ID3D11SamplerState *ss = m_Textures[i].GetSamplerState();
        pDeviceContext->PSSetSamplers(i, 1, &m_Textures[i].GetSamplerState());
        pDeviceContext->PSSetShaderResources(i, 1, &m_Textures[i].GetShaderResourceView());
    }

    pDeviceContext->IASetInputLayout(m_pInputLayout);
}
