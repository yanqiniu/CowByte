#include "MeshManager.h"
#include "GeometryCPU/Mesh.h"
#include "../Messaging/CBMessaging.h"
#include "GeometryCPU/MeshInstance.h"


MeshManager::MeshManager() :
    m_Meshes(128),
    m_MesheInstPtrs(8)
{

}

MeshManager::~MeshManager()
{
    ReleaseMeshesGPU();
}

Mesh* MeshManager::CPULoadMesh(const char *meshName)
{
    Mesh temp;
    if (!temp.LoadCPU(meshName))
        return nullptr;
    return AddMesh(temp);
}

Mesh* MeshManager::GetMeshPtr(UID meshId)
{
    return m_Meshes.Get(meshId);
}

// Insert mesh into map then return ptr to added mesh.
Mesh* MeshManager::AddMesh(const Mesh &toAdd)
{
    // Notice this take advantage of shallow copy of mesh,
    // that the temp toAdd object and inserted copy has the same Id.
    return m_Meshes.Insert(toAdd, toAdd.GetID());
}

// Insert MeshInstance pointer to m_MesheInstPtrs so that it is 
// sorted by MeshID small->large.
// This is so that MeshInstances with the same mesh references are 
// grouped together so we have less switching during drawing.
void MeshManager::AddMeshInstance(MeshInstance* pToAdd)
{
    // First acquire the proper MeshID.
    pToAdd->FindAndSetMeshID(*this);

    // Insert.
    for (size_t i = 0; i < m_MesheInstPtrs.Size(); ++i)
    {
        if (pToAdd->GetMeshID() >= m_MesheInstPtrs.peekat(i)->GetMeshID())
        {
            m_MesheInstPtrs.Insert(i + 1, pToAdd);
            return;
        }
    }
    // Empty of start of vector.
    m_MesheInstPtrs.Insert(0, pToAdd);
}

// Used upon MeshInstance creation to identify which Mesh
// it needs.
UID MeshManager::GetMeshID(const Filename &meshfn) const
{
    for (size_t i = 0; i < m_Meshes.Capacity(); ++i)
    {
        if (m_Meshes.Peek(i) != nullptr && 
            m_Meshes.Peek(i)->GetMeshName().Compare(const_cast<Filename&>(meshfn)) == 0)
        {
            return m_Meshes.Peek(i)->GetID();
        }
    }

    return INVALID_UID;
}

bool MeshManager::LoadMeshesGPU(ID3D11Device *pD3DDevice, ID3D11DeviceContext *pDeviceContext)
{
    bool res = true;
    for (size_t i = 0; i < m_Meshes.Capacity(); ++i)
    {
        if(m_Meshes.Peek(i) == nullptr)
            continue;

        res &= m_Meshes.Get(i)->InitializeGPU(pD3DDevice, pDeviceContext);
    }

    return res;
}

void MeshManager::ReleaseMeshesGPU()
{
    for (size_t i = 0; i < m_Meshes.Capacity(); ++i)
    {
        if (m_Meshes.Peek(i) == nullptr)
            continue;

        m_Meshes.Get(i)->ReleaseGPU();
    }
}

bool MeshManager::Update(const GameContext &context)
{
    return true;
}

void MeshManager::_HandleMessage(CBRefCountPtr<Message> &pMsg)
{
    if (pMsg->GetInstType() == Msg_RegisterDrawbleMeshInst::ClassTypeSpecifier())
    {
        MeshInstance *pMeshInst = static_cast<Msg_RegisterDrawbleMeshInst*>(pMsg.Get())->m_MeshInstPtr;
        AddMeshInstance(pMeshInst);
    }
}



