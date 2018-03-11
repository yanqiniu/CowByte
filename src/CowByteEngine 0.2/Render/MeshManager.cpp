#include "MeshManager.h"
#include "Mesh.h"


Mesh* MeshManager::CPULoadMesh(const char *meshName)
{
    Mesh* toRet = m_Meshes.Push_back(Mesh());
    toRet->ConfigureMesh(meshName);
    toRet->LoadContent();
    return toRet;
}

