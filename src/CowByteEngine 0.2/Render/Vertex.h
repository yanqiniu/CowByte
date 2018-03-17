#ifndef _VERTEX_H
#define _VERTEX_H

#include <d3d11.h>
#include <D3DX10.h>
#include <D3DX11.h>

#include "../Math/Vec3.h"


struct Vertex
{
    Vertex();
    Vertex(float px, float py, float pz,
           float nx, float ny, float nz,
           float r, float g, float b);
    ~Vertex();

    static D3D11_INPUT_ELEMENT_DESC InputDesc[3];
    Vec3 m_Pos;
    Vec3 m_Normal;
    D3DXCOLOR m_Color;
};


#endif // 

