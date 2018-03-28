#ifndef _TEXTURE_GPU_H
#define _TEXTURE_GPU_H
#include <d3d11_1.h>
#include "../../Utils/typedefs.h"
#include "../GeometryCPU/TextureCPU.h"
#include "GPURegisterLayout.h"
//#include "TextureManager.h"

class TextureGPU
{
    friend class TextureManager;
public:
    TextureGPU();
    ~TextureGPU();

    GPUTextureReg GetReg() const { return m_RegSlot; }
    ID3D11SamplerState*const & GetSamplerState() const { return m_pSamplerState; }
    ID3D11ShaderResourceView*const & GetShaderResourceView() const { return m_pShaderRscView; }

private:
    // Hidden, only TextureManager has access.
    bool LoadFromTextureCPU(ID3D11Device *pDevice, const TextureCPU &texCPU);
    void Release();

    Filename m_TexFileName;
    ID3D11SamplerState       *m_pSamplerState;
    ID3D11ShaderResourceView *m_pShaderRscView;
    GPUTextureReg m_RegSlot;
    UID m_UID;
};
#endif

