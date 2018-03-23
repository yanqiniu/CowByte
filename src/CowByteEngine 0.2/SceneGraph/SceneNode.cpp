#include "SceneNode.h"
#include "../Utils/CBVector.h"

SceneNode SceneNode::RootNode;

SceneNode::SceneNode():
    m_WorldTransform(Matrix4x4::Identity()),
    m_LocalTransform(Matrix4x4::Identity())
    //m_ChildrenNodes(4)
{

}

SceneNode::~SceneNode()
{

}

SceneNode* SceneNode::CreateSceneNodeThenAttach(SceneNode *parentPtr)
{
    SceneNode* toRet = new SceneNode();

    if (parentPtr != nullptr)
    {
        toRet->AttachTo_SceneNode_Parent(parentPtr);
    }
    return toRet;
}

// Walk scene graph branch upwards.
void SceneNode::UpdateWorldTransform()
{
    m_WorldTransform = m_LocalTransform;
    SceneNode *ptr = GetParentSceneNode();
    while (ptr != nullptr)
    {
        m_WorldTransform *= ptr->m_LocalTransform;
        ptr = ptr->GetParentSceneNode();
    }
}

// Given world transform, walk the tree down wards to update local transform to correct value.
// Notice that this uses a lot of inverse matrices so it's slow.
void SceneNode::UpdateLocalTransform()
{
    CBVector<SceneNode*> vectorFromParentToRoot(8);
    SceneNode *ptr = GetParentSceneNode();
    while (ptr != nullptr)
    {
        vectorFromParentToRoot.Push_back(ptr);
        ptr = ptr->GetParentSceneNode();
    }

    // Now we have walk the path backwards:
    Matrix4x4 res = m_WorldTransform;
    for (int i = vectorFromParentToRoot.Size() - 1; i >= 0; --i)
    {
        res *= vectorFromParentToRoot.peekat(i)->GetLocalTransform().Inversed();
    }
    
    m_LocalTransform = res;
}

// Convert a model space position to world space.
// Notice that this simply uses the cached m_WorldTransform,
// so make sure it's updated.
Vec3 SceneNode::CalculateWorldPosition(const Vec3& inPos)
{
    return inPos * m_WorldTransform;
}

void SceneNode::Translate(const Vec3& inPos)
{
    m_LocalTransform *= Matrix4x4::Translate(inPos);
    UpdateWorldTransform();
}

void SceneNode::Translate(float x, float y, float z)
{
    m_LocalTransform *= Matrix4x4::Translate(x, y, z);
    UpdateWorldTransform();
}

// Angle in degree.
void SceneNode::RotateX(float angle)
{
    m_LocalTransform *= Matrix4x4::RotationX(DEGREE_TO_RAD(angle));
    UpdateWorldTransform();
}

// Angle in degree.
void SceneNode::RotateY(float angle)
{
    m_LocalTransform *= Matrix4x4::RotationY(DEGREE_TO_RAD(angle));
    UpdateWorldTransform();
}

// Angle in degree.
void SceneNode::RotateZ(float angle)
{
    m_LocalTransform *= Matrix4x4::RotationZ(DEGREE_TO_RAD(angle));
    UpdateWorldTransform();
}

void SceneNode::Rotate(const Vec3 &axis, float angleInDegree)
{
    if (axis.SqLen() != 0)
    {
        m_LocalTransform *= Matrix4x4::Rotation(Quaternion(axis.Normalized(), DEGREE_TO_RAD(angleInDegree)));
    }
    else
    {
        m_LocalTransform *= Matrix4x4::Rotation(Quaternion(axis, DEGREE_TO_RAD(angleInDegree)));
    }
    UpdateWorldTransform();
}

void SceneNode::RotateLocal(const Vec3 &axis, float angleInDegree)
{
    if (axis.SqLen() != 0)
    {
        m_LocalTransform *= m_LocalTransform.LocalRotation(Quaternion(axis.Normalized(), DEGREE_TO_RAD(angleInDegree)));
    }
    else
    {
        m_LocalTransform *= m_LocalTransform.LocalRotation(Quaternion(axis, DEGREE_TO_RAD(angleInDegree)));
    }
    UpdateWorldTransform();
}

void SceneNode::Scale(float x, float y, float z)
{
    m_LocalTransform *= Matrix4x4::Scale(x, y, z);
    UpdateWorldTransform();
}

void SceneNode::Scale(float scale)
{
    Scale(scale, scale, scale);
}

void SceneNode::LookAt(const SceneNode &target, Vec3 up)
{
    m_WorldTransform = m_WorldTransform.LookAt(target.GetWorldTransform().GetPosition(), up);
    UpdateLocalTransform();
}

bool SceneNode::Update(const GameContext &context)
{
    return true;
}

void SceneNode::_HandleMessage(CBRefCountPtr<Message> &pMsg)
{

}
