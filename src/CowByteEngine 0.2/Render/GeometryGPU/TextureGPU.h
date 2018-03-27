#ifndef _TEXTURE_GPU_H
#define _TEXTURE_GPU_H
#include <d3d11_1.h>
#include "../../Utils/typedefs.h"
//#include "TextureManager.h"

class TextureGPU
{
    friend class TextureManager;
public:
    TextureGPU();
    ~TextureGPU();

    ID3D11SamplerState*const & GetSamplerState() const { return m_pSamplerState; }
    ID3D11ShaderResourceView*const & GetShaderResourceView() const { return m_pShaderRscView; }

private:
    // Hidden, only TextureManager has access.
    bool LoadFromFile(ID3D11Device *pDevice, const char *filename);
    void Release();

    Filename m_TexFileName;
    ID3D11SamplerState       *m_pSamplerState;
    ID3D11ShaderResourceView *m_pShaderRscView;
    UID m_UID;
};
#endif

