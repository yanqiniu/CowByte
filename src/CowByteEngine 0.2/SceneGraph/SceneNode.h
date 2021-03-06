#ifndef _SCENENODE_H
#define _SCENENODE_H

#include "../Math/CBMath.h"
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

    Vec3 CalculateWorldPosition(const Vec3& inPos);

    void Translate(const Vec3& inPos);
    void Translate(float x, float y, float z);
    void RotateX(float angle);
    void RotateY(float angle);
    void RotateZ(float angle);
    void Rotate(const Vec3 &axis, float angleInDegree);
    void RotateLocal(const Vec3 &axis, float angleInDegree);
    void Scale(float x, float y, float z);
    void Scale(float scale);
    void LookAt(const Vec3 &target, Vec3 up);
    void LookAt(const SceneNode &target, Vec3 up);

    const Matrix4x4& GetLocalTransform() const { return m_LocalTransform; }
    const Matrix4x4& GetWorldTransform() const { return m_WorldTransform; }

    virtual bool Update(const GameContext &context) override;
    virtual void _HandleMessage(CBRefCountPtr<Message> &pMsg) override;

private:
    void UpdateWorldTransform();
    void UpdateLocalTransform();

    Matrix4x4 m_WorldTransform; // Model Space -> World Space, yea this is cached.
    Matrix4x4 m_LocalTransform; // Local Space -> Parent Space
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
