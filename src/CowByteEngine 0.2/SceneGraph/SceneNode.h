#ifndef _SCENENODE_H
#define _SCENENODE_H

#include "../Math/Vec3.h"
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

    const Matrix4x4 &GetLocalTransform();
    const Matrix4x4 &GetWorldTransform();

    void UpdateWorldTransform();

    // Convert a model space position to world space.
    // Notice that this simply uses the cached m_WorldTransform,
    // so make sure it's updated.
    Vec3 CalculateWorldPosition(const Vec3& inPos);

    void AttachTo_SceneNode_Parent(SceneNode* parentPtr);

    void Translate(const Vec3& inPos);
    // void Rotate();
    // void Scale();

    virtual bool Update(const GameContext &context) override;
    virtual void _HandleMessage(CBRefCountPtr<Message> pMsg) override;

private:
    Matrix4x4 m_WorldTransform; // Model Space -> World Space, yea this is cached.
    Matrix4x4 m_LocalTransform; // Local Space -> Parent Space

    CBVector<SceneNode*> m_ChildrenNodes; // Yes, this smells like duplicate for Component::m_Components;
                                          // This is needed for doing SceneGraph only traversal (don't bother other components).
                                          // It won't be needed if one day we use handle that can tell types to manage pointers.
};


inline const Matrix4x4 & SceneNode::GetLocalTransform()
{
    return m_LocalTransform;
}

inline const Matrix4x4 & SceneNode::GetWorldTransform()
{
    return m_WorldTransform;
}

#endif
