
#ifndef _TEXTURE_MANAGER_H
#define _TEXTURE_MANAGER_H

#include "../../Utils/CBVector.h"
#include "TextureGPU.h"
#include "../../Utils/CBMap.h"
#include "../../Memory/CBMemory.h"
#include "../../Core/Component.h"
#include "../GeometryCPU/TextureCPU.h"

class TextureManager : public Component
{
public:
    CBMEM_OVERLOAD_NEW_DELETE(TextureManager)
    TextureManager();
    ~TextureManager();

    UID LoadFromTextureCPU(ID3D11Device *pDevice, const TextureCPU &texCPU);
    TextureGPU* GetTexture(UID id);
    const TextureGPU* PeekTexture(UID id) const;
    void Release();
    virtual bool Update(const GameContext &context) override;
    virtual void _HandleMessage(CBRefCountPtr<Message> &pMsg) override;

private:
    CBMap<TextureGPU> m_Textures;
};
#endif

