#include "MaterialCPU.h"
#include "../../Utils/CBFile.h"
#include "../../Utils/CBString.h"
#include "../../Utils/CBStringOps.h"
#include "../../Utils/CBDebug.h"
#include "../../Utils/CBPath.h"



MaterialCPU::MaterialCPU() :
    m_TextureCPUs(4)
{
}


MaterialCPU::~MaterialCPU()
{
}

bool MaterialCPU::LoadFromFile(const char *filepath)
{

    /* A sample material file.
    MAT

    # Diffuse Color.
    1.0 1.0 1.0 1.0

    # Diffuse Strength.
    1.0

    # Specular Strength.
    1.0

    # Shininess
    3.0

    # Albedo map
    brick_albedo.dds

    # Normal map
    brick_normal.dds

    # Specular map
    none

    # Vertex shader
    default_vs.cso

    # Pixel shader
    NormalMapped_ps.cso

    */

    CBString<256> line;
    CBFile matFile(filepath);

    // Check file type.
    if (!matFile.GetNextNonEmptyLine(line, false))
    {
        DbgERROR("Failed getting file type [%s]", filepath);
        return false;
    }
    line.Strip(StripMode::ALL);
    if (line.Compare("MAT") != 0)
    {
        DbgERROR("Not a Material file! [%s]\n", filepath);
        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    // Get diffuse color.
    float tempR, tempG, tempB, tempA;
    line.Clear();
    if (!matFile.GetNextNonEmptyLine(line, false))
    {
        DbgERROR("Failed getting diffuse color [%s]", filepath);
        return false;
    }
    line.Strip(StripMode::ALL);
    char* marker = line.Get();
    if (!CBStringOps::GetNextFloat32(marker, tempR, ' ') ||
        !CBStringOps::GetNextFloat32(marker, tempG, ' ') ||
        !CBStringOps::GetNextFloat32(marker, tempB, ' ') ||
        !CBStringOps::GetNextFloat32(marker, tempA, ' '))
    {
        DbgERROR("Failed getting diffuse color [%s]", filepath);
        return false;
    }
    m_ConstBufCPU.m_ColorDiffuse.Set(tempR, tempG, tempB, tempA);

    //////////////////////////////////////////////////////////////////////////
    // Get diffuse strength.
    line.Clear();
    if (!matFile.GetNextNonEmptyLine(line, false))
    {
        DbgERROR("Failed getting diffuse strength [%s]", filepath);
        return false;
    }
    line.Strip(StripMode::ALL);
    marker = line.Get();
    if (!CBStringOps::GetNextFloat32(marker, tempR, ' '))
    {
        DbgERROR("Failed getting diffuse strength [%s]", filepath);
        return false;
    }
    m_ConstBufCPU.m_DiffuseStrength = tempR;

    //////////////////////////////////////////////////////////////////////////
    // Get specular strength.
    line.Clear();
    if (!matFile.GetNextNonEmptyLine(line, false))
    {
        DbgERROR("Failed getting specular strength [%s]", filepath);
        return false;
    }
    line.Strip(StripMode::ALL);
    marker = line.Get();
    if (!CBStringOps::GetNextFloat32(marker, tempR, ' '))
    {
        DbgERROR("Failed getting specular strength [%s]", filepath);
        return false;
    }
    m_ConstBufCPU.m_SpecularStrength = tempR;

    //////////////////////////////////////////////////////////////////////////
    // Get shininess.
    line.Clear();
    if (!matFile.GetNextNonEmptyLine(line, false))
    {
        DbgERROR("Failed getting shininess [%s]", filepath);
        return false;
    }
    line.Strip(StripMode::ALL);
    marker = line.Get();
    if (!CBStringOps::GetNextFloat32(marker, tempR, ' '))
    {
        DbgERROR("Failed getting shininess [%s]", filepath);
        return false;
    }
    m_ConstBufCPU.m_Shininess = tempR;

    //////////////////////////////////////////////////////////////////////////
    // Get albedo map.
    line.Clear();
    if (!matFile.GetNextNonEmptyLine(line, false))
    {
        DbgERROR("Failed getting Albedo map [%s]", filepath);
        return false;
    }
    line.Strip(StripMode::ALL);
    if (!line.Compare("none") == 0)
        m_TextureCPUs.Push_back(TextureCPU(Filename(line.Get()), TextureType::Albedo));

    //////////////////////////////////////////////////////////////////////////
    // Get normal map.
    line.Clear();
    if (!matFile.GetNextNonEmptyLine(line, false))
    {
        DbgERROR("Failed getting Normal map [%s]", filepath);
        return false;
    }
    line.Strip(StripMode::ALL);
    if (!line.Compare("none") == 0)
        m_TextureCPUs.Push_back(TextureCPU(Filename(line.Get()), TextureType::Normal));

    //////////////////////////////////////////////////////////////////////////
    // Get specular map.
    line.Clear();
    if (!matFile.GetNextNonEmptyLine(line, false))
    {
        DbgERROR("Failed getting Albedo map [%s]", filepath);
        return false;
    }
    line.Strip(StripMode::ALL);
    if (!line.Compare("none") == 0)
        m_TextureCPUs.Push_back(TextureCPU(Filename(line.Get()), TextureType::Specular));

    //////////////////////////////////////////////////////////////////////////
    // Get vertex shader.
    line.Clear();
    if (!matFile.GetNextNonEmptyLine(line, false))
    {
        DbgERROR("Failed getting vertex shader [%s]", filepath);
        return false;
    }
    line.Strip(StripMode::ALL);
    m_NameVertexShader.Set(line.Get());

    //////////////////////////////////////////////////////////////////////////
    // Get pixel shader.
    line.Clear();
    if (!matFile.GetNextNonEmptyLine(line, false))
    {
        DbgERROR("Failed getting pixel shader [%s]", filepath);
        return false;
    }
    line.Strip(StripMode::ALL);
    m_NamePixelShader.Set(line.Get());

    return true;
}

const CBVector<TextureCPU>& MaterialCPU::GetTextureCPUs() const
{
    return m_TextureCPUs;
}
