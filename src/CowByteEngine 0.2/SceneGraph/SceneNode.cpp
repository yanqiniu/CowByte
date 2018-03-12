#include "SceneNode.h"
#include "../Math/Vec3.h"

SceneNode SceneNode::RootNode;

SceneNode::SceneNode():
    m_WorldTransform(Matrix4x4::Identity()),
    m_LocalTransform(Matrix4x4::Identity()),
    m_ChildrenNodes(4)
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

void SceneNode::UpdateWorldTransform()
{
    m_WorldTransform = m_LocalTransform;
    SceneNode *ptr = m_pParentSceneNode;
    while (ptr != nullptr)
    {
        m_WorldTransform *= ptr->m_LocalTransform;
        ptr = m_pParentSceneNode->m_pParentSceneNode;
    }
}

Vec3 SceneNode::CalculateWorldPosition(const Vec3& inPos)
{
    return inPos * m_WorldTransform;
}

void SceneNode::AttachTo_SceneNode_Parent(SceneNode* parentPtr)
{
    AttachTo_NonSceneNode_Parent(parentPtr);
    DbgAssert(parentPtr == m_pParentSceneNode, "SceneNode pointer component attached to must be the same as parent ptr!");
    m_pParentSceneNode = parentPtr;

    parentPtr->m_ChildrenNodes.Push_back(this);
}

void SceneNode::Translate(const Vec3& inPos)
{
    m_LocalTransform *= Matrix4x4::Translate(inPos);
}
