#ifndef _CAMERA_H
#define _CAMERA_H

#include "../Core/Component.h"
#include "../Math/CBMath.h"
#include "SceneNode.h"

// Projection Camera
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

private:
    Matrix4x4 m_ViewProjMatrix;     // World->Clip
                                    //TODO: add message handler to update this when m_WorldTransform has been changed.
    Matrix4x4 m_ProjectionMatrix;

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
