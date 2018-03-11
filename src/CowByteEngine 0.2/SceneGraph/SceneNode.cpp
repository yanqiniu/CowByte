#include "SceneNode.h"
#include "../Math/Vec3.h"

SceneNode SceneNode::RootNode;

SceneNode::SceneNode():
    m_WorldTransform(Matrix4x4::Identity()),
    m_LocalTransform(Matrix4x4::Identity()),
    m_ChildrenNodes(4),
    m_pParentNode(nullptr)
{

}

SceneNode::~SceneNode()
{

}

void SceneNode::SetParent(SceneNode *parentPtr)
{
    // TODO: walk scene graph to make sure there isn't a cycle.

    m_pParentNode = parentPtr;
    parentPtr->m_ChildrenNodes.Push_back(this);
}

inline const Matrix4x4 & SceneNode::GetLocalTransform()
{
    return m_LocalTransform;
}

inline const Matrix4x4 & SceneNode::GetWorldTransform()
{
    return m_WorldTransform;
}

void SceneNode::UpdateWorldTransform()
{
    m_WorldTransform = m_LocalTransform;
    SceneNode *ptr = m_pParentNode;
    while (ptr != nullptr)
    {
        m_WorldTransform *= ptr->m_LocalTransform;
        ptr = m_pParentNode->m_pParentNode;
    }
}

Vec3 SceneNode::CalculateWorldPosition(const Vec3& inPos)
{
    return inPos * m_WorldTransform;
}

