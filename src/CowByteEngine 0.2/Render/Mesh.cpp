#include "Mesh.h"
#include "../Utils/CBPath.h"
#include "../Utils/CBFile.h"
#include "../Utils/CBDebug.h"

int Mesh::g_IDCounter = 0;

Mesh::Mesh() :
    m_Vertices(8),
    m_Indices(8),
    m_nVertices(0),
    m_nTriangles(0),
    m_UID(g_IDCounter++)
{
}


Mesh::~Mesh()
{
}

bool Mesh::LoadContent(const char* meshName)
{
    Filepath meshFilePath;
    Path::GenerateAssetPath(meshFilePath, "meshes", meshName);
    //DbgINFO("Mesh path set to: [%s]", m_MeshFilePath.Get());

    CBFile meshFile(meshFilePath.Get());
    CBString<64> temp;

    // Check if it's a Mesh file
    if (!meshFile.GetNextNonEmptyLine(temp.Get(), temp.Capacity(), false))
    {
        DbgERROR("Failed getting file type [%s]", meshFilePath.Get());
        return false;
    }
    temp.Strip(StripMode::ALL);
    if (temp.Compare("MESH") != 0)
    {
        printf("Not a mesh file! [s]\n", meshFilePath.Get());
        return false;
    }

    // Read the position buffer file.
    temp.Clear();
    if (!meshFile.GetNextNonEmptyLine(temp.Get(), temp.Capacity(), false))
    {
        DbgERROR("Failed getting pos buf file in [%s].", meshFilePath.Get());
        return false;
    }
    temp.Strip(StripMode::ALL);
    if (!ReadPosBufFile(temp.Get()))
    {
        DbgERROR("Failed reading in pos buffer [%s].", temp.Get());
        return false;
    }

    // Read the index buffer file.
    temp.Clear();
    if (!meshFile.GetNextNonEmptyLine(temp.Get(), temp.Capacity(), false))
    {
        DbgERROR("Failed getting index buf file in [%s].", meshFilePath.Get());
        return false;
    }
    temp.Strip(StripMode::ALL);
    if (!ReadIndexBufFile(temp.Get()))
    {
        DbgERROR("Failed reading in index buffer [%s].", temp.Get());
        return false;
    }

    m_MeshName = Filename(meshName);
    return true;

}

bool Mesh::ReadPosBufFile(const char *filepath)
{
    Filepath tempPath;
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
    m_nVertices = static_cast<size_t>(atoi(temp.Get()));

    // Initialize vertex array.
    float tempFloat, tempX, tempY, tempZ;;
    for (size_t i = 0; i < m_nVertices; ++i)
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
        
        m_Vertices.Push_back(Vertex())->m_Pos = Vec3(0.5f * tempX, 0.5f * tempY, 0.5f  * tempZ + 0.5f);
    }

    for (size_t i = 0; i < m_nVertices; ++i)
    {
        DbgINFO ("Vertex %d [%f, %f, %f]", i, m_Vertices[i].m_Pos.X(), m_Vertices[i].m_Pos.Y(), m_Vertices[i].m_Pos.Z());
    }
    
    return true;

}

bool Mesh::ReadIndexBufFile(const char *filepath)
{
    Filepath tempPath;
    Path::GenerateAssetPath(tempPath, "meshes", filepath);
    CBFile indexBufFile(tempPath.Get());

    CBString<64> temp;
    if (!indexBufFile.GetNextNonEmptyLine(temp.Get(), temp.Capacity(), false))
    {
        DbgERROR("Failed getting file type [%s]", filepath);
        return false;
    }
    temp.Strip(StripMode::ALL);
    if (temp.Compare("INDEX_BUF") != 0)
    {
        DbgERROR("Not a index_buf file! [%s]\n", filepath);
        return false;
    }

    // Get num of triangles.
    temp.Clear();
    if (!indexBufFile.GetNextNonEmptyLine(temp.Get(), temp.Capacity(), false))
    {
        DbgERROR("Failed getting num of vertices [%s]", filepath);
        return false;
    }
    temp.Strip(StripMode::ALL);
    m_nTriangles = static_cast<size_t>(atoi(temp.Get()));

    // Initialize vertex array.
    int tempInt;
    for (size_t i = 0; i < m_nTriangles; ++i)
    {
        temp.Clear();
        if (!indexBufFile.GetNextNonEmptyLine(temp.Get(), temp.Capacity(), false))
        {
            DbgERROR("Not enough triangles in [%s].", filepath);
            return false;
        }
        temp.Strip();
        char *marker = temp.Get();
        for (int j = 0; j < 3; ++j)
        {
            if (!CBStringOps::GetNextInt32(marker, tempInt, ' '))
            {
                DbgERROR("Not enough values for triangle[%d] in [%s].", i, filepath);
                return false;
            }
            m_Indices.Push_back(static_cast<WORD>(tempInt));
        }
    }

    for (size_t i = 0; i < m_nTriangles; ++i)
    {
        DbgINFO("Triangle %d [%d, %d, %d]", i, m_Indices[i * 3], m_Indices[i * 3 + 1], m_Indices[i * 3 + 2]);
    }

    return true;
}
