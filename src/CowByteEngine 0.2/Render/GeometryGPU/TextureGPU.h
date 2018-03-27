#ifndef _TEXTURE_GPU_H
#define _TEXTURE_GPU_H
#include <d3d11_1.h>

class TextureGPU
{
public:
    TextureGPU();
    ~TextureGPU();

    bool LoadFromFile(ID3D11Device *pDevice, const char *filename);
    void Release();

    ID3D11SamplerState*const & GetSamplerState() const { return m_pSamplerState; }
    ID3D11ShaderResourceView*const & GetShaderResourceView() const { return m_pShaderRscView; }

private:
    ID3D11SamplerState       *m_pSamplerState;
    ID3D11ShaderResourceView *m_pShaderRscView;
};
#endif

