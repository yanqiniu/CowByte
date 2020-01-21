#ifndef _DRAW_CALL_GAMEOBJECT_H
#define _DRAW_CALL_GAMEOBJECT_H

#include "DrawCall.h"
#include "../../Math/CBMath.h"
#include "../GeometryCPU/Mesh.h"

class DrawCallGameObject : public DrawCall
{
public:
    DrawCallGameObject();
    ~DrawCallGameObject();

    void SetData(const Matrix4x4& worldMatrix, const UID& meshID);

private:
    Matrix4x4 m_WorldMatrix;
    UID       m_MeshId;

};

#endif
