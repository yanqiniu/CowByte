#ifndef _TEXTURE_CPU_H
#define _TEXTURE_CPU_H
#include "../../Memory/CBMemory.h"
#include "../../Utils/typedefs.h"

enum TextureType
{
    Albedo,
    Normal,
    Specular
};

class TextureCPU
{
public:
    TextureCPU(const Filename &fn, TextureType type);
    ~TextureCPU();

    const Filename& GetFilename() const { return m_Filename; }
    TextureType GetType() const { return m_Type; }

private:
    Filename m_Filename;
    TextureType m_Type;
};

#endif

