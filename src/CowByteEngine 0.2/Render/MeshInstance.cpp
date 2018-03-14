#include "MeshInstance.h"
#include "MeshManager.h"

MeshInstance::MeshInstance() :
    m_MeshFileName(),
    m_MeshID(INVALID_UID)
{

}

MeshInstance::MeshInstance(const char *filename) :
    m_MeshFileName(),
    m_MeshID(INVALID_UID)
{
    SetFileName(filename);
}

MeshInstance::~MeshInstance()
{

}

void MeshInstance::SetFileName(const char *filename)
{
    m_MeshFileName.Set(filename);
}

bool MeshInstance::FindAndSetMeshID(const MeshManager& meshManager)
{
    m_MeshID = meshManager.GetMeshID(m_MeshFileName);
    if (m_MeshID == INVALID_UID)
        return false;
    return true;  
}

