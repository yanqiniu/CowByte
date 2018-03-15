#include "Camera.h"

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
    m_ProjectionMatrix(Matrix4x4::PerspectiveProjection(ar, fovy, np, fp))
{

}

Camera::~Camera()
{

}

bool Camera::Update(const GameContext &context)
{
    return true;
}

void Camera::_HandleMessage(CBRefCountPtr<Message> pMsg)
{

}
