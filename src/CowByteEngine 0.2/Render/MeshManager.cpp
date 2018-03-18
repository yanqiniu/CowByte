#include "MeshManager.h"
#include "Mesh.h"
#include "../Messaging/CBMessaging.h"
#include "MeshInstance.h"


MeshManager::MeshManager() :
    m_Meshes(8),
    m_MesheInstPtrs(8)
{

}

MeshManager::~MeshManager()
{

}

Mesh* MeshManager::CPULoadMesh(const char *meshName)
{
    Mesh *toRet = AddMesh(Mesh());
    toRet->LoadContent(meshName);
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
        else // meshId > m_Meshes[m].GetID()
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

    // Empty or start of vector.
    return m_Meshes.Insert(0, toAdd);
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
    for (size_t i = 0; i < m_Meshes.Size(); ++i)
    {
        if (m_Meshes.peekat(i).GetMeshName().Compare(const_cast<Filename&>(meshfn)) == 0)
        {
            return m_Meshes.peekat(i).GetID();
        }
    }

    return INVALID_UID;
}

bool MeshManager::Update(const GameContext &context)
{
    return true;
}

void MeshManager::_HandleMessage(CBRefCountPtr<Message> &pMsg)
{
    if (pMsg->type == MessageType::MsgType_RegisterDrawbleMeshInstance)
    {
        
        MeshInstance *pMeshInst = static_cast<Msg_RegisterDrawbleMeshInst*>(pMsg.Get())->m_MeshInstPtr;
        AddMeshInstance(pMeshInst);
    }
}



