#ifndef _MESH_H
#define _MESH_H
#include "../Utils/CBString.h"
#include "../Utils/CBVector.h"
#include "../Utils/typedefs.h"
#include "Vertex.h"

class Mesh
{
public:
    Mesh();
    ~Mesh();


    bool              LoadContent(const char* meshName);
    UID               GetID() const;
    const Filename&   GetMeshName() const;
    CBVector<Vertex>& GetVertices();
    CBVector<WORD>&   GetIndices();
    size_t            GetNumVertices() const;
    size_t            GetNumTriangles() const;
    bool              IsLoaded() const;

private:
    bool ReadPosBufFile(const char *filepath);
    bool ReadIndexBufFile(const char *filepath);
    bool ReadNormalBufFile(const char *filepath);

    static int g_IDCounter;

    Filename         m_MeshName;
    CBVector<Vertex> m_Vertices;
    CBVector<WORD>   m_Indices;
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

inline bool Mesh::IsLoaded() const
{
    return m_bIsLoaded;
}



#endif // 
