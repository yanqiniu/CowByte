#ifndef _MESH_INSTANCE_H
#define _MESH_INSTANCE_H

#include "../../Utils/typedefs.h"
#include "../../Core/Component.h"

class MeshManager;

class MeshInstance : public Component
{
public:
    MeshInstance();
    MeshInstance(const char *filename);
    ~MeshInstance();

    void SetFileName( const char *filename);
    bool FindAndSetMeshID(const MeshManager& meshManager);
    UID GetMeshID() { return m_MeshID; }

    virtual bool Update(const GameContext &context) override;
    virtual void _HandleMessage(CBRefCountPtr<Message> &pMsg) override;

private:
    Filename m_MeshFileName;
    UID m_MeshID;

};

#endif
