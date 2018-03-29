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
    struct LightData
    {
        LightData();

        Vec3    m_Position;
        Vec3    m_Direction;
        CBColor m_Color;
        INT32   m_Type; // Use this instead of enum because enum size is not consistent.
        FLOAT32 m_Radius;
    };

public:

    Light();
    virtual ~Light();
    
    virtual bool Update(const GameContext &context) override;
    virtual void _HandleMessage(CBRefCountPtr<Message> &pMsg) override {};

    bool InitializeAmbient(const CBColor& color);
    bool InitializeDirectional(const CBColor& color);
    bool InitializePoint(const CBColor& color, float radius);

    void RegisterLight();

    const Vec3      GetPos() const { return m_Data.m_Position; }
    const Vec3      GetDir() const { return m_Data.m_Direction; }
    const CBColor   GetColor() const { return m_Data.m_Color; }
    const LightType GetType() const { return static_cast<LightType>(m_Data.m_Type); }
    const float     GetRadius() const { return m_Data.m_Radius; }
    const LightData GetData() const { return m_Data; }

private:
    LightData m_Data;
};
#endif // _LIGHT_H

