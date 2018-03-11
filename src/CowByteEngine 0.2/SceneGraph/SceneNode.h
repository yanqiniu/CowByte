#ifndef _SCENENODE_H
#define _SCENENODE_H

#include "../Math/Matrix4x4.h"
#include "../Core/Component.h"
#include "../Utils/CBVector.h"

class SceneNode : public Component
{
public:
    // Root node that is ALWAYS gonna sit at the center of world,
    // With all transform set to identity.
    static SceneNode RootNode;

    SceneNode();
    ~SceneNode();

    void SetParent(SceneNode *parentPtr);

    const Matrix4x4 &GetLocalTransform();
    const Matrix4x4 &GetWorldTransform();

    void UpdateWorldTransform();

    // Convert a model space position to world space.
    // Notice that this simply uses the cached m_WorldTransform,
    // so make sure it's updated.
    Vec3 CalculateWorldPosition(const Vec3& inPos);

private:
    Matrix4x4 m_WorldTransform; // Model Space -> World Space, yea this is cached.
    Matrix4x4 m_LocalTransform; // Local Space -> Parent Space

    CBVector<SceneNode*> m_ChildrenNodes; // This is needed for doing SceneGraph only traversal (don't bother other components).
                                          // It won't be needed if one day we use handle that can tell types to manage pointers.
    SceneNode *m_pParentNode;


};

#endif
