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

    CBString<64> m_MeshName;

    bool LoadContent(const char* meshName);
    UID GetID() const;

private:
    bool ReadPosBufFile(const char *filepath);

    static int g_IDCounter;
    CBVector<Vertex> m_Vertices;
    size_t m_nVertices;
    UID m_UID;
};

inline UID Mesh::GetID() const
{
    return m_UID;
}

#endif // 
