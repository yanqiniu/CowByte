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

// fovy should be in degree.
Camera::Camera(float ar, float fovy, float np, float fp) :
    m_WorldToCamaraSpace(Matrix4x4::Identity()),
    m_AspectRatio(ar),
    m_FOVy(DEGREE_TO_RAD(fovy)),
    m_NearPlane(np),
    m_FarPlane(fp),
    m_ProjectionMatrix(Matrix4x4::PerspectiveProjection(ar, DEGREE_TO_RAD(fovy), np, fp))
{

}

Camera::~Camera()
{

}

bool Camera::Update(const GameContext &context)
{
    return true;
}

void Camera::_HandleMessage(CBRefCountPtr<Message> &pMsg)
{

}
