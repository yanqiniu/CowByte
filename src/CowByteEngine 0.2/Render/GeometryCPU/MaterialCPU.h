#ifndef _MATERIAL_CPU_H
#define _MATERIAL_CPU_H

#include "../../Utils/typedefs.h"
#include "../../Memory/CBMemory.h"
#include "../Other/CBColor.h"

struct PerMatConstBufCPU
{
    CBColor m_ColorDiffuse;
    CBColor m_ColorSpecular;
};

enum TexMapType
{
    Albedo,
    Normal,
    Specular
};

class MaterialCPU
{
public:
    CBMEM_OVERLOAD_NEW_DELETE(MaterialCPU)
    MaterialCPU();
    ~MaterialCPU();

    bool LoadFromFile(const char *filepath);
    // Getters:
    const Filename& GetMapName(TexMapType type) const;
    const Filename& GetVSName() const { return m_NameVertexShader; };
    const Filename& GetPSName() const { return m_NamePixelShader; }
    const PerMatConstBufCPU& GetConstBufferCPU() const{ return m_ConstBufCPU; }

private:
    // TODO: right now, a material has to have each of these maps.
    Filename m_NameAlbedoMap;
    Filename m_NameNormalMap;
    Filename m_NameSpecularMap;

    Filename m_NameVertexShader;
    Filename m_NamePixelShader;

    PerMatConstBufCPU m_ConstBufCPU;
};

#endif

