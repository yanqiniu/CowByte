#ifndef _CAMERA_H
#define _CAMERA_H

#include "../Core/Component.h"
#include "../Math/CBMath.h"
#include "SceneNode.h"

// Perspective Camera
class Camera : public Component
{
public:
    Camera();
    Camera(float ar, float fovy, float np, float fp);
    ~Camera();
    void UpdateVPMatrix(); // Update m_ViewProjMatrix.

    Matrix4x4 GetProjectionMatrix() const;
    Matrix4x4 GetViewProjMatrix() const;
    bool Update(const GameContext &context);
    void _HandleMessage(CBRefCountPtr<Message> &pMsg);

    float GetNearPlane() const { return m_NearPlane; }
    float GetFarPlane() const { return m_FarPlane; }

private:
    Matrix4x4 m_ViewProjMatrix;     // World->Clip
    Matrix4x4 m_ProjectionMatrix;   // View->Clip

    float m_AspectRatio;
    float m_FOVy;
    float m_NearPlane;
    float m_FarPlane;
};

inline void Camera::UpdateVPMatrix()
{
    m_ViewProjMatrix = GetParentSceneNode()->GetWorldTransform().Inversed() * m_ProjectionMatrix;
}

inline Matrix4x4 Camera::GetProjectionMatrix() const
{
    return m_ProjectionMatrix;
}

inline Matrix4x4 Camera::GetViewProjMatrix() const
{
    return m_ViewProjMatrix;
}



#endif
