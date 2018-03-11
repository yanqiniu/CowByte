#ifndef _MESH_MANAGER_H
#define _MESH_MANAGER_H

#include "../Core/Component.h"
#include "Mesh.h"


class MeshManager : Component
{
public:
    // Return pointer to the mesh loaded.
    Mesh* CPULoadMesh(const char *meshName);

private:
    CBVector<Mesh> m_Meshes;// Meshes, not Mesh instances.
};

#endif
