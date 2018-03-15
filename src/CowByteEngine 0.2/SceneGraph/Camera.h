#ifndef _CAMERA_H
#define _CAMERA_H

#include "../Core/Component.h"
#include "../Math/Matrix4x4.h"
#include "SceneNode.h"

// Projection Camera
class Camera : public Component
{
public:
    Camera();
    Camera(float ar, float fovy, float np, float fp);
    ~Camera();
    void UpdateWToCMatrix(); // Update m_WorldToCamaraSpace.

    Matrix4x4 GetWToCMatrix() const;
    Matrix4x4 GetProjectionMatrix() const;
    bool Update(const GameContext &context);
    void _HandleMessage(CBRefCountPtr<Message> pMsg);

private:
    Matrix4x4 m_WorldToCamaraSpace; //Inverse of SceneNode::m_WorldTransform. 
                                    //TODO: add message handler to update this when m_WorldTransform has been changed.
    Matrix4x4 m_ProjectionMatrix;

    float m_AspectRatio;
    float m_FOVy;
    float m_NearPlane;
    float m_FarPlane;
};

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


#endif
