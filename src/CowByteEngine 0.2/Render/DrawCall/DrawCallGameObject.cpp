#include "DrawCallGameObject.h"

DrawCallGameObject::DrawCallGameObject()
{
    m_Type = DrawCall::DrawCall_GameObject;
}

DrawCallGameObject::~DrawCallGameObject()
{

}

void DrawCallGameObject::SetData(const Matrix4x4& worldMatrix, const UID& meshID)
{
    m_WorldMatrix = worldMatrix;
    m_MeshId = meshID;
}
