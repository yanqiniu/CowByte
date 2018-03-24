#ifndef _VERTEX_H
#define _VERTEX_H

#include <d3d11.h>

#include "../Math/Vec3.h"
#include "../Math/Vec2.h"
#include "../Memory/CBMemory.h"

struct Vertex
{
    CBMEM_OVERLOAD_NEW_DELETE(Vertex)

    Vertex();
    ~Vertex();

    static D3D11_INPUT_ELEMENT_DESC InputDesc[3];
    Vec3 m_Pos;
    Vec3 m_Normal;
    Vec2 m_TexCoord;
};


#endif // 

