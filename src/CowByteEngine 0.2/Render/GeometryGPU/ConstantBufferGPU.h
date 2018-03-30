#ifndef _CONSTANT_BUFFER_GPU_H
#define _CONSTANT_BUFFER_CPU_H
#include <d3d11_1.h>
#include "../../Utils/CBDebug.h"

using namespace DirectX;

struct ConstantBufferGPU
{
    virtual bool CreateBuffers(ID3D11Device *pDevice) = 0;
    virtual void SetBuffers(ID3D11DeviceContext *pDeviceContext) = 0;
};

struct PerAppConstBufGPU
{

};

struct PerFrameConstBufGPU : public ConstantBufferGPU
{
    PerFrameConstBufGPU() :
        m_ViewProjMatrix(nullptr)
    {}

    virtual bool CreateBuffers(ID3D11Device *pDevice) override
    {
        D3D11_BUFFER_DESC constantBufferDesc;
        ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));
        constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        constantBufferDesc.CPUAccessFlags = 0;
        constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

        constantBufferDesc.ByteWidth = sizeof(Matrix4x4);
        if (!ResultNotFailed(pDevice->CreateBuffer(&constantBufferDesc, nullptr, &m_ViewProjMatrix)))
        {
            DbgERROR("Failed creating buffer!");
            return false;
        }

        return true;
    }

    virtual void SetBuffers(ID3D11DeviceContext *pDeviceContext)
    {
        pDeviceContext->VSSetConstantBuffers(GPUConstantsReg_VS::ViewProjMatrix, 1, &m_ViewProjMatrix);
    }

    //////////////////////////////////////////////////////////////////////////

    ID3D11Buffer *m_ViewProjMatrix;
};

struct PerObjectConstBufGPU : public ConstantBufferGPU
{
    PerObjectConstBufGPU() :
        m_WorldMatrix(nullptr)
    {}

    virtual bool CreateBuffers(ID3D11Device *pDevice) override
    {
        D3D11_BUFFER_DESC constantBufferDesc;
        ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));
        constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        constantBufferDesc.CPUAccessFlags = 0;
        constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

        constantBufferDesc.ByteWidth = sizeof(Matrix4x4);
        if (!ResultNotFailed(pDevice->CreateBuffer(&constantBufferDesc, nullptr, &m_WorldMatrix)))
        {
            DbgERROR("Failed creating buffer!");
            return false;
        }

        return true;
    }

    virtual void SetBuffers(ID3D11DeviceContext *pDeviceContext)
    {
        pDeviceContext->VSSetConstantBuffers(GPUConstantsReg_VS::ObjectWorldMatrix, 1, &m_WorldMatrix);
    }

    //////////////////////////////////////////////////////////////////////////

    ID3D11Buffer *m_WorldMatrix;
};

#endif // 
