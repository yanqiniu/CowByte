#include "MeshManager.h"
#include "Mesh.h"


MeshManager::MeshManager() :
    m_Meshes(8)
{

}

MeshManager::~MeshManager()
{

}

Mesh* MeshManager::CPULoadMesh(const char *meshName)
{
    Mesh *toRet = AddMesh(Mesh());
    toRet->ConfigureMesh(meshName);
    toRet->LoadContent();
    return toRet;
}

Mesh* MeshManager::GetMeshPtr(UID meshId)
{
    // NOTICE:
    // This function uses binary search so 
    // make sure the mesh vector is sorted 
    // by m_UID small->large.
    // You don't have to worry about this
    // if you stick to using AddMesh(), 
    // instead of push_back() or insert()
    // to m_Meshes directly.

    size_t l = 0; 
    size_t r = m_Meshes.Size() - 1;

    while (l <= r)
    {
        size_t m = (r + l) / 2;

        // Check middle:
        if (m_Meshes[m].GetID() == meshId)
            return &m_Meshes[m];
        else if (meshId < m_Meshes[m].GetID())
            r = m - 1;
        else // m_Meshes[m].GetID() > m
            l = m + 1;
    }

    return nullptr;
}

// Insert mesh into vector, sorted by m_ID small->large.
// Then return ptr to added mesh.
Mesh* MeshManager::AddMesh(const Mesh &toAdd)
{
    for (size_t i = 0; i < m_Meshes.Size(); ++i)
    {
        if (toAdd.GetID() >= m_Meshes[i].GetID())
        {
            return m_Meshes.Insert(i + 1, toAdd);
        }
    }
}

