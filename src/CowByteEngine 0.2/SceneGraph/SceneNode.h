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

    // Create a new scene node on the heap and attach it to the parent specified.
    static SceneNode* CreateSceneNodeThenAttach(SceneNode *parentPtr);

    void SetParent(SceneNode *parentPtr);

    const Matrix4x4 &GetLocalTransform();
    const Matrix4x4 &GetWorldTransform();

    void UpdateWorldTransform();

    // Convert a model space position to world space.
    // Notice that this simply uses the cached m_WorldTransform,
    // so make sure it's updated.
    Vec3 CalculateWorldPosition(const Vec3& inPos);

    void AttachTo_SceneNode_Parent(SceneNode* parentPtr);

private:
    Matrix4x4 m_WorldTransform; // Model Space -> World Space, yea this is cached.
    Matrix4x4 m_LocalTransform; // Local Space -> Parent Space

    CBVector<SceneNode*> m_ChildrenNodes; // Yes, this smells like duplicate for Component::m_Components;
                                          // This is needed for doing SceneGraph only traversal (don't bother other components).
                                          // It won't be needed if one day we use handle that can tell types to manage pointers.
};

#endif
