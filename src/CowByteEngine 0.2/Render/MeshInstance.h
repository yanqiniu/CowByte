#ifndef _MESH_INSTANCE_H
#define _MESH_INSTANCE_H

#include "../Utils/typedefs.h"
#include "../Core/Component.h"

class MeshInstance : public Component
{
public:
    MeshInstance();
    ~MeshInstance();

private:
    UID m_MeshID;
};

#endif
