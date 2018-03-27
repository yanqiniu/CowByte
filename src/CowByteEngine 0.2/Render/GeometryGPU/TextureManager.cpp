#include "TextureManager.h"
#include "../../Utils/CBDebug.h"
#include "../../Utils/CBStringOps.h"


TextureManager::TextureManager() :
    m_Textures(256) // Supports 256 textures max.
{
}


TextureManager::~TextureManager()
{
}

UID TextureManager::LoadTextureFromFile(ID3D11Device *pDevice, const char *filename)
{
    // See if exists.
    for (size_t i = 0; i < m_Textures.Capacity(); ++i)
    {
        if (m_Textures.PeekAt(i) == nullptr)
            continue;
        if (m_Textures.PeekAt(i)->m_TexFileName.Compare(filename) == 0)
            return m_Textures.PeekAt(i)->m_UID;
    }

    TextureGPU temp;
    temp.LoadFromFile(pDevice, filename);
    if (m_Textures.Insert(temp, temp.m_UID) == nullptr)
    {
        DbgERROR("Failed adding texture to manager.");
        return INVALID_UID;
    }
    return temp.m_UID;
}

TextureGPU* TextureManager::GetTexture(UID id)
{
    return m_Textures.Get(id);
}

const TextureGPU* TextureManager::PeekTexture(UID id) const
{
    return m_Textures.Peek(id);
}

void TextureManager::Release()
{
    for (size_t i = 0; i < m_Textures.Capacity(); ++i)
    {
        if (m_Textures.PeekAt(i) == nullptr)
            continue;

        m_Textures.At(i)->Release();
    }
}

bool TextureManager::Update(const GameContext &context)
{
    return true;
}

void TextureManager::_HandleMessage(CBRefCountPtr<Message> &pMsg)
{

}
