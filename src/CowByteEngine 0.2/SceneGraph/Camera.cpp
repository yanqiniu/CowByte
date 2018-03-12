#include "Camera.h"
#include "SceneNode.h"

void Camera::UpdateWToCMatrix()
{
    m_WorldToCamaraSpace = m_pParentSceneNode->GetWorldTransform().Inverse();
}
