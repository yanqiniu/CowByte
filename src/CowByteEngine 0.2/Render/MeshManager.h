#ifndef _MESH_MANAGER_H
#define _MESH_MANAGER_H

#include "../Core/Component.h"
#include "GeometryCPU/Mesh.h"
#include "../Utils/typedefs.h"
#include "../Utils/CBMap.h"
class ID3D11Device;
class ID3D11DeviceContext;
class TextureManager;

class MeshManager : public Component
{
public:
    MeshManager();
    ~MeshManager();
    
    Mesh* CPULoadMesh(const char *meshName); // Return pointer to the mesh loaded.
    Mesh* GetMeshPtr(UID meshId);
    UID   GetMeshID(const Filename &meshfn) const;
    const CBVector<MeshInstance*> &GetMeshInsts() const;

    bool GPULoadMeshes(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext, TextureManager *pTexManager);
    void ReleaseMeshesGPU();

    bool Update(const GameContext &context) override;

private:
    virtual void _HandleMessage(CBRefCountPtr<Message> &pMsg) override;
    Mesh* AddMesh(const Mesh &toAdd);
    void AddMeshInstance(MeshInstance* pToAdd);

    CBMap<Mesh> m_Meshes;
    CBVector<MeshInstance*> m_MesheInstPtrs;
};

inline const CBVector<MeshInstance*>& MeshManager::GetMeshInsts() const
{
    return m_MesheInstPtrs;
}

#endif
