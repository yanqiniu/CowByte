#ifndef _MESH_H
#define _MESH_H
#include "UObject.h"

class Mesh : UObject
{
public:
    Mesh();
    ~Mesh();

    char m_MeshFilePath[64];

    bool ConfigureMesh( const char* meshName );
    bool Initialize();
    bool LoadContent();
};

#endif // 
