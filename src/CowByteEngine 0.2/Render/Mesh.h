#ifndef _MESH_H
#define _MESH_H
#include "../Utils/CBString.h"
#include "../Utils/CBVector.h"

class Vertex;

class Mesh
{
public:
    Mesh();
    ~Mesh();

    CBString<256> m_MeshFilePath;

    bool ConfigureMesh( const char* meshName );
    bool LoadContent();

private:
    CBVector<Vertex> m_Vertices;
    size_t m_NumVertices;

    bool ReadPosBufFile(const char *filepath);
};

#endif // 
