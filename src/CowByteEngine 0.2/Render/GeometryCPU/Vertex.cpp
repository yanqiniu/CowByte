
#include "Vertex.h"

D3D11_INPUT_ELEMENT_DESC Vertex::InputDesc[4] = 
{
{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,                D3D11_INPUT_PER_VERTEX_DATA, 0 },
{ "NORMAL",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vec3),     D3D11_INPUT_PER_VERTEX_DATA, 0 },
{ "TANGENT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vec3) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },
{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, sizeof(Vec3) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// Default is origin pos, white color.
Vertex::Vertex() :
    m_Pos(0, 0, 0, 1.0f),
    m_Normal(0, 0, 0, 1.0f),
    m_TexCoord(0, 0)
{

}

Vertex::~Vertex()
{
}
