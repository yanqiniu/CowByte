#ifndef _MESH_H
#define _MESH_H
#include "../Utils/CBString.h"
#include "Vertex.h"

class Mesh
{
public:
    Mesh();
    ~Mesh();

    CBString<256> m_MeshFilePath;

    bool ConfigureMesh( const char* meshName );
    bool Initialize();
    bool LoadContent();

private:
    Vertex * m_pVertices;
    size_t m_NumVertices;

    bool ReadPosBufFile(const char *filepath);
};

#endif // 
