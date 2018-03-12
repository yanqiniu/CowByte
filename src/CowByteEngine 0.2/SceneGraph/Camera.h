#ifndef _CAMERA_H
#define _CAMERA_H

#include "../Core/Component.h"
#include "../Math/Matrix4x4.h"

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

private:
    Matrix4x4 m_WorldToCamaraSpace; //Inverse of SceneNode::m_WorldTransform. 
                                    //TODO: add message handler to update this when m_WorldTransform has been changed.
    Matrix4x4 m_ProjectionMatrix;

    float m_AspectRatio;
    float m_FOVy;
    float m_NearPlane;
    float m_FarPlane;
};

#endif
