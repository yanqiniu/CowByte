#ifndef _MATERIAL_GPU_H
#define _MATERIAL_GPU_H

#include <d3d11_1.h>
#include "TextureGPU.h"
#include "../../Utils/CBVector.h"

class MaterialGPU
{
public:
    MaterialGPU();
    ~MaterialGPU();

    bool LoadFromMaterialCPU(/*const MaterialCPU &matcpu*/);
    void SetAsActive(ID3D11DeviceContext *pDeviceContext);

private:
    CBVector<TextureGPU> m_Textures;
    ID3D11VertexShader  *m_pShaderVertex;
    ID3D11PixelShader   *m_pShaderPixel;
    ID3D11InputLayout   *m_pInputLayout; // Currently this is the same for all shaders.
};
#endif

