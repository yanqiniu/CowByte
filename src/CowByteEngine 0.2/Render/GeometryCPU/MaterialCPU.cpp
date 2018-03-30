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

    # Specular Color.
    1.0 1.0 1.0 1.0

    # Albedo map
    cube_albedo.dds

    # Normal map
    cube_normal.dds

    # Specular map
    cube_specular.dds

    # Vertex shader
    defualt_vs.hlsl

    # Pixel shader
    default_ps.hlsl

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

    // Get specular color.
    line.Clear();
    if (!matFile.GetNextNonEmptyLine(line, false))
    {
        DbgERROR("Failed getting specular color [%s]", filepath);
        return false;
    }
    line.Strip(StripMode::ALL);
    marker = line.Get();
    if (!CBStringOps::GetNextFloat32(marker, tempR, ' ') ||
        !CBStringOps::GetNextFloat32(marker, tempG, ' ') ||
        !CBStringOps::GetNextFloat32(marker, tempB, ' ') ||
        !CBStringOps::GetNextFloat32(marker, tempA, ' '))
    {
        DbgERROR("Failed getting specular color [%s]", filepath);
        return false;
    }
    m_ConstBufCPU.m_ColorSpecular.Set(tempR, tempG, tempB, tempA);

    // TODO: handle non-existent maps.
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

    // Get vertex shader.
    line.Clear();
    if (!matFile.GetNextNonEmptyLine(line, false))
    {
        DbgERROR("Failed getting vertex shader [%s]", filepath);
        return false;
    }
    line.Strip(StripMode::ALL);
    m_NameVertexShader.Set(line.Get());

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
