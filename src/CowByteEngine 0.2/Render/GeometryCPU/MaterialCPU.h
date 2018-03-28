#ifndef _MATERIAL_CPU_H
#define _MATERIAL_CPU_H

#include "../../Utils/typedefs.h"
#include "../../Memory/CBMemory.h"
#include "../Other/CBColor.h"
#include "TextureCPU.h"
#include "../../Utils/CBVector.h"

struct PerMatConstBufCPU
{
    CBColor m_ColorDiffuse;
    CBColor m_ColorSpecular;
};

class MaterialCPU
{
public:
    CBMEM_OVERLOAD_NEW_DELETE(MaterialCPU)
    MaterialCPU();
    ~MaterialCPU();

    bool LoadFromFile(const char *filepath);
    // Getters:
    const CBVector<TextureCPU>& GetTextureCPUs() const;
    const Filename& GetVSName() const { return m_NameVertexShader; };
    const Filename& GetPSName() const { return m_NamePixelShader; }
    const PerMatConstBufCPU& GetConstBufferCPU() const{ return m_ConstBufCPU; }

private:
    // TODO: right now, a material has to have each of these maps.
    CBVector<TextureCPU> m_TextureCPUs;

    Filename m_NameVertexShader;
    Filename m_NamePixelShader;

    PerMatConstBufCPU m_ConstBufCPU;
};

#endif

