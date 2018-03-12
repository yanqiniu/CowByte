#ifndef _CAMERA_H
#define _CAMERA_H

#include "../Core/Component.h"
#include "../Math/Matrix4x4.h"

class Camera : public Component
{
public:
    void UpdateWToCMatrix(); // Update m_WorldToCamaraSpace.
private:
    Matrix4x4 m_WorldToCamaraSpace; //Inverse of SceneNode::m_WorldTransform. 
                                    //TODO: add message handler to update this when m_WorldTransform has been changed.
    float m_AspectRatio;
    float m_FOVy;
    float m_NearPlane;
    float m_FarPlane;
};

#endif
