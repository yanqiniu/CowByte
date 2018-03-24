#ifndef _INDEX_BUFFER_GPU_H
#define _INDEX_BUFFER_GPU_H
#include <d3d11_1.h>
#include "../../Utils/CBVector.h"
#include "../GeometryCPU/Vertex.h"

class IndexBufferGPU
{
public:
    IndexBufferGPU();
    ~IndexBufferGPU();

    inline ID3D11Buffer* Get();
    bool InitFromWORDVector(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext, CBVector<WORD> &indices);
    void Release();
    void SetAsActive(ID3D11DeviceContext *pDeviceContext) const;
    UINT Count() const { return m_IndexCount; }

private:
    ID3D11Buffer * m_pIndexBuffer;
    UINT m_IndexCount;
};

//////////////////////////////////////////////////////////////////////////
// Inline functions.

ID3D11Buffer* IndexBufferGPU::Get()
{
    return m_pIndexBuffer;
}


#endif

