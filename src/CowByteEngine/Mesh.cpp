#include "Mesh.h"
#include "CBPath.h"
#include "CBFile.h"
#include "CBDebug.h"

Mesh::Mesh() :
    UObject()
{
}


Mesh::~Mesh()
{
}

// Configure file path and such. This should run before
// LoadContent(), therefore before Initialize().
bool Mesh::ConfigureMesh(const char* meshName)
{
    Path::GenerateAssetPath(m_MeshFilePath, "meshes", meshName);
    DbgINFO("Mesh path set to: [%s]", m_MeshFilePath.Get());
    return true;
}

bool Mesh::Initialize()
{
    LoadContent();
    return true;
}

bool Mesh::LoadContent()
{
    CBFile meshFile(m_MeshFilePath.Get());
    CBString<64> temp;

    // Check if it's a Mesh file
    if (!meshFile.GetNextNonEmptyLine(temp.Get(), temp.Capacity(), false))
    {
        DbgERROR("Failed getting file type [%s]", m_MeshFilePath.Get());
        return false;
    }
    temp.Strip(StripMode::ALL);
    if (temp.Compare("MESH") != 0)
    {
        printf("Not a mesh file! [s]\n", m_MeshFilePath.Get());
        return false;
    }

    // Read the position buffer file.
    temp.Clear();
    if (!meshFile.GetNextNonEmptyLine(temp.Get(), temp.Capacity(), false))
    {
        DbgERROR("Failed getting pos buf file in [%s].", m_MeshFilePath.Get());
        return false;
    }
    temp.Strip(StripMode::ALL);
    if (!ReadPosBufFile(temp.Get()))
    {
        DbgERROR("Failed reading in pos buffer [%s].", temp.Get());
        return false;
    }

    return true;

}

bool Mesh::ReadPosBufFile(const char *filepath)
{
    CBString<256> tempPath;
    Path::GenerateAssetPath(tempPath, "meshes", filepath);
    CBFile posBufFile(tempPath.Get());

    CBString<64> temp;
    if (!posBufFile.GetNextNonEmptyLine(temp.Get(), temp.Capacity(), false))
    {
        DbgERROR("Failed getting file type [%s]", filepath);
        return false;
    }
    temp.Strip(StripMode::ALL);
    if (temp.Compare("POS_BUF") != 0)
    {
        DbgERROR("Not a pos_buf file! [%s]\n", filepath);
        return false;
    }

    // Get num of vertices.
    temp.Clear();
    if (!posBufFile.GetNextNonEmptyLine(temp.Get(), temp.Capacity(), false))
    {
        DbgERROR("Failed getting num of vertices [%s]", filepath);
        return false;
    }
    temp.Strip(StripMode::ALL);
    int intbuf;
    m_NumVertices = static_cast<size_t>(atoi(temp.Get()));

    // Initialize vertex array.
    // TODO: Allocator
    m_pVertices = new Vertex[m_NumVertices];
    float tempFloat, tempX, tempY, tempZ;;
    for (size_t i = 0; i < m_NumVertices; ++i)
    {
        temp.Clear();
        if (!posBufFile.GetNextNonEmptyLine(temp.Get(), temp.Capacity(), false))
        {
            DbgERROR("Not enough vertices in [%s].", filepath);
            return false;
        }
        temp.Strip();
        char *marker = temp.Get();
        for (int j = 0; j < 3; ++j)
        {
            if (!CBStringOps::GetNextFloat32(marker, tempFloat, ' '))
            {
                DbgERROR("Not enough values for vertex[%d] in [%s].", i, filepath);
                return false;
            }

            switch (j)
            {
            case 0: tempX = tempFloat; break;
            case 1: tempY = tempFloat; break;
            case 2: tempZ = tempFloat; break;
            }
        }
        
        m_pVertices[i].m_Pos = Vec3(tempX, tempY, tempZ);
    }

    for (size_t i = 0; i < m_NumVertices; ++i)
    {
        DbgINFO ("Vertex %d [%f, %f, %f]", i, m_pVertices[i].m_Pos.X(), m_pVertices[i].m_Pos.Y(), m_pVertices[i].m_Pos.Z());
    }
    
    delete[] m_pVertices;

    return true;

}
