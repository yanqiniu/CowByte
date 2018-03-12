#ifndef _MESH_H
#define _MESH_H
#include "../Utils/CBString.h"
#include "../Utils/CBVector.h"
#include "../Utils/typedefs.h"

class Vertex;

class Mesh
{
public:
    Mesh();
    ~Mesh();


    bool LoadContent(const char* meshName);
    UID GetID() const;
    const Filename& GetMeshName() const;

private:
    bool ReadPosBufFile(const char *filepath);

    static int g_IDCounter;
    Filename m_MeshName;
    CBVector<Vertex> m_Vertices;
    size_t m_nVertices;
    UID m_UID;
};

inline UID Mesh::GetID() const
{
    return m_UID;
}


inline const Filename& Mesh::GetMeshName() const
{
    return m_MeshName;
}


#endif // 
