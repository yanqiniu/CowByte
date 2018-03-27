#ifndef _MATERIAL_GPU_H
#define _MATERIAL_GPU_H

#include <d3d11_1.h>
#include "TextureGPU.h"
#include "../../Utils/CBVector.h"
#include "../../Utils/typedefs.h"
#include "../GeometryCPU/MaterialCPU.h"

class MaterialGPU
{
public:
    MaterialGPU();
    ~MaterialGPU();

    bool LoadFromMaterialCPU(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext, const MaterialCPU &matcpu);
    void SetAsActive(ID3D11DeviceContext *pDeviceContext) const;
    void Release();

private:
    CBVector<TextureGPU> m_Textures;
    ID3D11VertexShader  *m_pShaderVertex;
    ID3D11PixelShader   *m_pShaderPixel;
    ID3D11InputLayout   *m_pInputLayout; // Currently this is the same for all shaders.
    ID3D11Buffer        *m_pConstBuf;
};
#endif

