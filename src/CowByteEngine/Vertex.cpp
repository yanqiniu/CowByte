
#include "Vertex.h"

D3D11_INPUT_ELEMENT_DESC Vertex::InputDesc[2] = 
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,            D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vec3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// Default is origin pos, white color.
Vertex::Vertex() :
    m_Pos(Vec3(0, 0, 0, 1.0f)),
    m_Color(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))
{

}


Vertex::Vertex(float x, float y, float z, float r, float g, float b) :
    m_Pos(Vec3(x, y, z, 1.0f)),
    m_Color(D3DXCOLOR(r, g, b, 1.0f))
{
}

Vertex::~Vertex()
{
}
