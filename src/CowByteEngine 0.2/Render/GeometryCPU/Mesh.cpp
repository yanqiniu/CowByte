#include "Mesh.h"
#include "../../Utils/CBPath.h"
#include "../../Utils/CBFile.h"
#include "../../Utils/CBDebug.h"


Mesh::Mesh() :
    m_Vertices(8),
    m_Indices(8),
    m_nVertices(0),
    m_nTriangles(0),
    m_UID(INVALID_UID),
    m_bIsLoaded(false)
{
}


Mesh::~Mesh()
{
}

// This should be called after cpu load.
bool Mesh::InitializeGPU(ID3D11Device *pD3DDevice, ID3D11DeviceContext *pDeviceContext, TextureManager *pTexManager)
{
    bool result = true;
    result &= m_VertexBuf.InitFromVertexVector(pD3DDevice, pDeviceContext, m_Vertices);
    result &= m_IndexBuf.InitFromWORDVector(pD3DDevice, pDeviceContext, m_Indices);
    result &= m_MaterialGPU.LoadFromMaterialCPU(pD3DDevice, pDeviceContext, m_MaterialCPU, pTexManager);

    return result;
}

void Mesh::ReleaseGPU()
{
    m_VertexBuf.Release();
    m_IndexBuf.Release();
    m_MaterialGPU.Release();
}

// CPU  read and load.
bool Mesh::LoadCPU(const char* meshName)
{
    Filepath meshFilePath;
    CBPath::GenerateAssetPath(meshFilePath, "meshes", meshName);
    //DbgINFO("Mesh path set to: [%s]", m_MeshFilePath.Get());

    CBFile meshFile(meshFilePath.Get());
    CBString<128> temp;

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

    // Read the normal buffer file.
    temp.Clear();
    if (!meshFile.GetNextNonEmptyLine(temp.Get(), temp.Capacity(), false))
    {
        DbgERROR("Failed getting normal buf file in [%s].", meshFilePath.Get());
        return false;
    }
    temp.Strip(StripMode::ALL);
    if (!ReadNormalBufFile(temp.Get()))
    {
        DbgERROR("Failed reading in normal buffer [%s].", temp.Get());
        return false;
    }

    // Read the normal buffer file.
    temp.Clear();
    if (!meshFile.GetNextNonEmptyLine(temp.Get(), temp.Capacity(), false))
    {
        DbgERROR("Failed getting uv buf file in [%s].", meshFilePath.Get());
        return false;
    }
    temp.Strip(StripMode::ALL);
    if (!ReadUVBufFile(temp.Get()))
    {
        DbgERROR("Failed reading in uv buffer [%s].", temp.Get());
        return false;
    }

    // Read the normal buffer file.
    temp.Clear();
    if (!meshFile.GetNextNonEmptyLine(temp.Get(), temp.Capacity(), false))
    {
        DbgERROR("Failed getting material file in [%s].", meshFilePath.Get());
        return false;
    }
    temp.Strip(StripMode::ALL);
    Filepath tempPath;
    CBPath::GenerateAssetPath(tempPath, "materials", temp.Get());
    if(!m_MaterialCPU.LoadFromFile(tempPath.Get()))
    {
        DbgERROR("Failed loading matcpu in file in [%s].", meshFilePath.Get());
        return false;
    }

    m_MeshName = Filename(meshName);
    m_bIsLoaded = true;

    DbgINFO("Finished Loading Mesh[%s].", meshName);
    return true;
}

bool Mesh::ReadPosBufFile(const char *filepath)
{
    Filepath tempPath;
    CBPath::GenerateAssetPath(tempPath, "meshes", filepath);
    CBFile posBufFile(tempPath.Get());

    CBString<128> temp;
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
        
        m_Vertices.Push_back(Vertex())->m_Pos = Vec3(tempX, tempY, tempZ);
    }

    //for (size_t i = 0; i < m_nVertices; ++i)
    //{
    //    DbgINFO ("Vertex %d [%f, %f, %f]", i, m_Vertices[i].m_Pos.X(), m_Vertices[i].m_Pos.Y(), m_Vertices[i].m_Pos.Z());
    //}

    static UID uidCounter = 0;
    m_UID = uidCounter++;
    
    return true;

}

bool Mesh::ReadIndexBufFile(const char *filepath)
{
    Filepath tempPath;
    CBPath::GenerateAssetPath(tempPath, "meshes", filepath);
    CBFile indexBufFile(tempPath.Get());

    CBString<128> temp;
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

    //for (size_t i = 0; i < m_nTriangles; ++i)
    //{
    //    DbgINFO("Triangle %d [%d, %d, %d]", i, m_Indices[i * 3], m_Indices[i * 3 + 1], m_Indices[i * 3 + 2]);
    //}

    return true;
}

// This must be called after ReadPosBufFile().
bool Mesh::ReadNormalBufFile(const char *filepath)
{
    Filepath tempPath;
    CBPath::GenerateAssetPath(tempPath, "meshes", filepath);
    CBFile normBufFile(tempPath.Get());

    CBString<128> temp;
    if (!normBufFile.GetNextNonEmptyLine(temp.Get(), temp.Capacity(), false))
    {
        DbgERROR("Failed getting file type [%s]", filepath);
        return false;
    }
    temp.Strip(StripMode::ALL);
    if (temp.Compare("NORMAL_BUF") != 0)
    {
        DbgERROR("Not a normal_buf file! [%s]\n", filepath);
        return false;
    }

    // Get num of normals.
    temp.Clear();
    if (!normBufFile.GetNextNonEmptyLine(temp.Get(), temp.Capacity(), false))
    {
        DbgERROR("Failed getting num of normals [%s]", filepath);
        return false;
    }
    temp.Strip(StripMode::ALL);
    if (static_cast<size_t>(atoi(temp.Get())) != m_nVertices || 
        m_Vertices.Size() != m_nVertices)
    {
        DbgERROR("Num of normals != Num of vertices [%s]!", filepath);
        return false;
    }

    // Fill in normals.
    float tempFloat, tempX, tempY, tempZ;;
    for (size_t i = 0; i < m_nVertices; ++i)
    {
        temp.Clear();
        if (!normBufFile.GetNextNonEmptyLine(temp.Get(), temp.Capacity(), false))
        {
            DbgERROR("Not enough normals in [%s].", filepath);
            return false;
        }
        temp.Strip();
        char *marker = temp.Get();
        for (int j = 0; j < 3; ++j)
        {
            if (!CBStringOps::GetNextFloat32(marker, tempFloat, ' '))
            {
                DbgERROR("Not enough values for normal[%d] in [%s].", i, filepath);
                return false;
            }

            switch (j)
            {
            case 0: tempX = tempFloat; break;
            case 1: tempY = tempFloat; break;
            case 2: tempZ = tempFloat; break;
            }
        }

        m_Vertices.at(i).m_Normal.Set(tempX, tempY, tempZ, 0.0f);
    }

    //for (size_t i = 0; i < m_nVertices; ++i)
    //{
    //    DbgINFO("Normal %d [%f, %f, %f]", i, m_Vertices[i].m_Normal.X(), m_Vertices[i].m_Normal.Y(), m_Vertices[i].m_Normal.Z());
    //}

    return true;
}

bool Mesh::ReadUVBufFile(const char *filepath)
{
    Filepath tempPath;
    CBPath::GenerateAssetPath(tempPath, "meshes", filepath);
    CBFile uvBufFile(tempPath.Get());

    CBString<128> temp;
    if (!uvBufFile.GetNextNonEmptyLine(temp.Get(), temp.Capacity(), false))
    {
        DbgERROR("Failed getting file type [%s]", filepath);
        return false;
    }
    temp.Strip(StripMode::ALL);
    if (temp.Compare("UV_BUF") != 0)
    {
        DbgERROR("Not a normal_buf file! [%s]\n", filepath);
        return false;
    }

    // Get num of textcoord.
    temp.Clear();
    if (!uvBufFile.GetNextNonEmptyLine(temp.Get(), temp.Capacity(), false))
    {
        DbgERROR("Failed getting num of textcoords [%s]", filepath);
        return false;
    }
    temp.Strip(StripMode::ALL);
    if (static_cast<size_t>(atoi(temp.Get())) != m_nVertices ||
        m_Vertices.Size() != m_nVertices)
    {
        DbgERROR("Num of textcoords != Num of vertices [%s]!", filepath);
        return false;
    }

    // Fill in normals.
    float tempFloat, tempX, tempY;
    for (size_t i = 0; i < m_nVertices; ++i)
    {
        temp.Clear();
        if (!uvBufFile.GetNextNonEmptyLine(temp.Get(), temp.Capacity(), false))
        {
            DbgERROR("Not enough textcoords in [%s].", filepath);
            return false;
        }
        temp.Strip();
        char *marker = temp.Get();
        for (int j = 0; j < 2; ++j)
        {
            if (!CBStringOps::GetNextFloat32(marker, tempFloat, ' '))
            {
                DbgERROR("Not enough values for normal[%d] in [%s].", i, filepath);
                return false;
            }

            switch (j)
            {
            case 0: tempX = tempFloat; break;
            case 1: tempY = tempFloat; break;
            }
        }

        m_Vertices.at(i).m_TexCoord.X = tempX;
        m_Vertices.at(i).m_TexCoord.Y = tempY;
    }

}
