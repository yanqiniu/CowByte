#ifndef _MESH_MANAGER_H
#define _MESH_MANAGER_H

#include "../Core/Component.h"
#include "Mesh.h"
#include "../Utils/typedefs.h"

class MeshManager : public Component
{
public:
    MeshManager();
    ~MeshManager();
    // Return pointer to the mesh loaded.
    Mesh* CPULoadMesh(const char *meshName);
    Mesh* GetMeshPtr(UID meshId);
    Mesh* AddMesh(const Mesh &toAdd);

private:
    CBVector<Mesh> m_Meshes;// Meshes, not Mesh instances.
};

#endif
