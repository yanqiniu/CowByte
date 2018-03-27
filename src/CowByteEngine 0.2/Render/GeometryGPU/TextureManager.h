
#ifndef _TEXTURE_MANAGER_H
#define _TEXTURE_MANAGER_H

#include "../../Utils/CBVector.h"
#include "TextureGPU.h"
#include "../../Utils/CBMap.h"

class TextureManager
{
public:
    TextureManager();
    ~TextureManager();

    UID LoadTextureFromFile(ID3D11Device *pDevice, const char *filename);
    TextureGPU* GetTexture(UID id);
    const TextureGPU* PeekTexture(UID id) const;
    void Release();
private:
    CBMap<TextureGPU> m_Textures;
};
#endif

