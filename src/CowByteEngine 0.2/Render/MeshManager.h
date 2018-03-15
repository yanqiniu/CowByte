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
    
    Mesh* CPULoadMesh(const char *meshName); // Return pointer to the mesh loaded.
    Mesh* GetMeshPtr(UID meshId);
    Mesh* AddMesh(const Mesh &toAdd);
    UID   GetMeshID(const Filename &meshfn) const;
    const CBVector<MeshInstance*> &GetMeshInsts() const;

    bool Update(const GameContext &context) override;

private:
    virtual void _HandleMessage(CBRefCountPtr<Message> pMsg) override;

    CBVector<Mesh> m_Meshes;// Meshes, not Mesh instances.
    CBVector<MeshInstance*> m_MesheInstPtrs;
};

inline const CBVector<MeshInstance*>& MeshManager::GetMeshInsts() const
{
    return m_MesheInstPtrs;
}

#endif
