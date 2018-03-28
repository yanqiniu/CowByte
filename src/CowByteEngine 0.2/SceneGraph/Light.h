#ifndef _LIGHT_H
#define _LIGHT_H
#include "../Render/Other/CBColor.h"
#include "../Core/Component.h"
#include "../Utils/typedefs.h"
#include "../Math/CBMath.h"

enum LightType
{
    AmbientLight = 0,
    DirectionalLight = 1,
    PointLight = 2
};

// CPU light.
class Light : public Component
{
public:

    Light();
    virtual ~Light();
    
    virtual bool Update(const GameContext &context) override;
    virtual void _HandleMessage(CBRefCountPtr<Message> &pMsg) override {};

    bool InitializeAmbient(const CBColor& color);
    bool InitializeDirectional(const CBColor& color, const Vec3& direction);
    bool InitializePoint(const CBColor& color, float radius);

    void RegisterLight();

private:

    Vec3    m_Direction;
    CBColor m_Color;
    INT32   m_Type; // Use this instead of enum because enum size is not consistent.
    FLOAT32 m_Radius;
};
#endif // _LIGHT_H

