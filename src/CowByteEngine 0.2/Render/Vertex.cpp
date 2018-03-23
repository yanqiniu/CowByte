
#include "Vertex.h"

D3D11_INPUT_ELEMENT_DESC Vertex::InputDesc[3] = 
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,                D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vec3),     D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vec3) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// Default is origin pos, white color.
Vertex::Vertex() :
    m_Pos(0, 0, 0, 1.0f),
    m_Normal(0, 0, 0, 1.0f),
    m_Color(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f)) // Initialize to purple.
{

}


Vertex::Vertex(float px, float py, float pz, 
               float nx, float ny, float nz, 
               float r, float g, float b) :
    m_Pos(px, py, pz, 1.0f),
    m_Normal(nx, ny, nz, 1.0f),
    m_Color(r, g, b, 1.0f)
{
}

Vertex::~Vertex()
{
}
