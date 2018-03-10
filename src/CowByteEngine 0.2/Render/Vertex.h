#ifndef _VERTEX_H
#define _VERTEX_H

#include <d3d11.h>
#include <D3DX10.h>
#include <D3DX11.h>

#include "../Math/Vec3.h"


struct Vertex
{
    Vertex();
    Vertex(float x, float y, float z, float r, float g, float b);
    ~Vertex();

    static D3D11_INPUT_ELEMENT_DESC InputDesc[2];
    Vec3 m_Pos;
    D3DXCOLOR m_Color;
};


#endif // 

