#ifndef _VERTEX_BUFFER_GPU_H
#define _VERTEX_BUFFER_GPU_H
#include <d3d11_1.h>
#include "../../Utils/CBVector.h"
#include "../GeometryCPU/Vertex.h"

class VertexBufferGPU
{
public:
    VertexBufferGPU();
    ~VertexBufferGPU();

    inline ID3D11Buffer* Get();
    bool InitFromVertexVector(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext, CBVector<Vertex> &vertices);
    void Release();
    void SetAsActive(ID3D11DeviceContext *pDeviceContext) const;

private:
    ID3D11Buffer *m_pVertexBuffer;
};
//////////////////////////////////////////////////////////////////////////
// Inline functions.

ID3D11Buffer* VertexBufferGPU::Get()
{
    return m_pVertexBuffer;
}


#endif

