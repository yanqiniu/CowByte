#include "LightManager.h"
#include "GPURegisterLayout.h"
#include "../../Utils/CBDebug.h"
#include "../../SceneGraph/Light.h"


using namespace DirectX;


LightManager::LightManager() :
    m_CPULights(4),
    m_GPULights(4)
{
}


LightManager::~LightManager()
{
}

bool LightManager::Update(const GameContext &context)
{
    return true;
}

void LightManager::_HandleMessage(CBRefCountPtr<Message> &pMsg)
{
    if (pMsg->GetInstType() == Msg_RegisterLight::ClassTypeSpecifier())
    {
        if (m_CPULights.Size() < g_nMaxLights)
        {
            m_CPULights.Push_back(MESSAGE_FROM_PTR(pMsg, Msg_RegisterLight)->m_pLight);
        }
        else
            DbgWARNING("Too many lights! Max = %d.", g_nMaxLights);
    }
}

bool LightManager::UpdateLightsGPU(ID3D11DeviceContext *pDeviceContext)
{
    for (size_t i = 0; i < m_CPULights.Size(); i++)
    {
        pDeviceContext->UpdateSubresource(m_GPULights.at(i), 0, nullptr, &m_CPULights.at(i)->GetData(), 0, 0);
    }
    pDeviceContext->PSSetConstantBuffers(GPUConstantsReg_PS::Light0, m_CPULights.Size(), &m_GPULights.at(0));
    return true;
}

// This only runs once.
bool LightManager::CreateLightsGPU(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext)
{
    static bool alreadyRun = false;
    if (alreadyRun == true)
        return true;
    alreadyRun = true;

    // Create and fill constant buffer.
    for (size_t i = 0; i < m_CPULights.Size(); i++)
    {
        D3D11_BUFFER_DESC constantBufferDesc;
        ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));
        constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        constantBufferDesc.ByteWidth = sizeof(Light);
        constantBufferDesc.CPUAccessFlags = 0;
        constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        ID3D11Buffer *pTemp;
        if (!ResultNotFailed(pDevice->CreateBuffer(&constantBufferDesc, nullptr, &pTemp)))
        {
            DbgERROR("Failed creating light on GPU!");
            return false;
        }
        m_GPULights.Push_back(pTemp);
    }
    //pDeviceContext->PSSetConstantBuffers(GPUConstantsReg_PS::Light0, m_CPULights.Size(), &m_GPULights.at(0));


    return true;
}
