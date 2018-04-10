#ifndef _LIGHT_MANAGER_H
#define _LIGHT_MANAGER_H
#include "../../Core/Component.h"
#include "../../Utils/CBVector.h"
#include <d3d11_1.h>

class Light;
class PerFrameConstBufGPU;
// This should be attached to Graphics.
class LightManager : public Component
{
public:
    LightManager();
    ~LightManager();
    virtual bool Update(const GameContext &context) override;
    virtual void _HandleMessage(CBRefCountPtr<Message> &pMsg) override;
    bool UpdateLightsGPU(ID3D11DeviceContext *pDeviceContext, PerFrameConstBufGPU *pConstBuf);
    bool CreateLightsGPU(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext);
private:
    CBVector<Light*> m_CPULights;
    CBVector<ID3D11Buffer*> m_GPULights;
};

#endif

