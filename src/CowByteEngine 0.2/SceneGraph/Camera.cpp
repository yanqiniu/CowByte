#include "Camera.h"
#include "SceneNode.h"

Camera::Camera() :
    m_WorldToCamaraSpace(Matrix4x4::Identity()),
    m_ProjectionMatrix(Matrix4x4::Identity()),
    m_AspectRatio(0),
    m_FOVy(0),
    m_NearPlane(0),
    m_FarPlane(0)
{

}

Camera::Camera(float ar, float fovy, float np, float fp) :
    m_WorldToCamaraSpace(Matrix4x4::Identity()),
    m_AspectRatio(ar),
    m_FOVy(fovy),
    m_NearPlane(np),
    m_FarPlane(fp),
    m_ProjectionMatrix(Matrix4x4::Projection(ar, fovy, np, fp))
{

}

Camera::~Camera()
{

}

inline void Camera::UpdateWToCMatrix()
{
    m_WorldToCamaraSpace = m_pParentSceneNode->GetWorldTransform().Inverse();
}

inline Matrix4x4 Camera::GetWToCMatrix() const
{
    return m_WorldToCamaraSpace;
}

inline Matrix4x4 Camera::GetProjectionMatrix() const
{
    return m_ProjectionMatrix;
}
