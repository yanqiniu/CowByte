#ifndef _MESH_H
#define _MESH_H
#include "UObject.h"
#include "CBString.h"

class Mesh : UObject
{
public:
    Mesh();
    ~Mesh();

    CBString<256> m_MeshFilePath;

    bool ConfigureMesh( const char* meshName );
    bool Initialize();
    bool LoadContent();
};

#endif // 
