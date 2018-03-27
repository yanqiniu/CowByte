#ifndef _MESH_H
#define _MESH_H
#include "../../Utils/CBString.h"
#include "../../Utils/CBVector.h"
#include "../../Utils/typedefs.h"
#include "../../Memory/CBMemory.h"
#include "../GeometryGPU/VertexBufferGPU.h"
#include "../GeometryGPU/IndexBufferGPU.h"
#include "../GeometryGPU/MaterialGPU.h"
#include "Vertex.h"

class ID3D11Device;
class ID3D11DeviceContext;
class TextureManager;

class Mesh
{
public:
    CBMEM_OVERLOAD_NEW_DELETE(Mesh)

    Mesh();
    ~Mesh();


    bool              LoadCPU(const char* meshName);
    UID               GetID() const;
    const Filename&   GetMeshName() const;
    CBVector<Vertex>& GetVertices();
    CBVector<WORD>&   GetIndices();
    size_t            GetNumVertices() const;
    size_t            GetNumTriangles() const;
    bool              IsLoaded() const;

    const VertexBufferGPU&  GetVertexBuffer() const;
    const IndexBufferGPU&   GetIndexBuffer() const;
    const MaterialGPU&      GetMaterial() const;

    bool InitializeGPU(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext, TextureManager *pTexManager);
    void ReleaseGPU();

private:
    bool ReadPosBufFile(const char *filepath);
    bool ReadIndexBufFile(const char *filepath);
    bool ReadNormalBufFile(const char *filepath);
    bool ReadUVBufFile(const char *filepath);

    static int g_IDCounter;

    Filename         m_MeshName;
    CBVector<Vertex> m_Vertices;
    CBVector<WORD>   m_Indices;
    VertexBufferGPU  m_VertexBuf;
    IndexBufferGPU   m_IndexBuf;
    MaterialCPU      m_MaterialCPU;
    MaterialGPU      m_MaterialGPU;
    size_t           m_nVertices;
    size_t           m_nTriangles;
    UID              m_UID;
    bool             m_bIsLoaded;
};

inline UID Mesh::GetID() const
{
    return m_UID;
}


inline const Filename& Mesh::GetMeshName() const
{
    return m_MeshName;
}

inline CBVector<Vertex>& Mesh::GetVertices() 
{
    return m_Vertices;
}
inline CBVector<WORD>& Mesh::GetIndices()
{
    return m_Indices;
}

inline size_t Mesh::GetNumVertices() const
{
    return m_nVertices;
}

inline size_t Mesh::GetNumTriangles() const
{
    return m_nTriangles;
}

inline const VertexBufferGPU&  Mesh::GetVertexBuffer() const
{
    return m_VertexBuf;
}
inline const IndexBufferGPU&  Mesh::GetIndexBuffer() const
{
    return m_IndexBuf;
}

inline const MaterialGPU& Mesh::GetMaterial() const
{
    return m_MaterialGPU;
}

inline bool Mesh::IsLoaded() const
{
    return m_bIsLoaded;
}



#endif // 
