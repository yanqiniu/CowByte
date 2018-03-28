#include "Light.h"
#include "../Utils/CBDebug.h"
#include "../Messaging/CBMessaging.h"



Light::Light() :
    m_Color(1.0f, 1.0f, 1.0f, 1.0f), // Default to white
    m_Direction(0.0f, -1.0f, 0.0f, 0.0f),
    m_Radius(1.0f),
    m_Type(-1)
{
}


Light::~Light()
{
}

bool Light::Update(const GameContext &context)
{
    return true;
}

bool Light::InitializeAmbient(const CBColor& color)
{
    // Yes, ambient light doesn't really need position from scene node, 
    // but it's needed for receiving messages.
    if (!HasSceneNodeParent())
    {
        DbgWARNING("Light must be attached to scene node!");
        return false;
    }
    m_Type = static_cast<INT32>(LightType::AmbientLight);
    m_Color = color;
    return true;
}

bool Light::InitializeDirectional(const CBColor& color, const Vec3& direction)
{
    // Yes, directional light doesn't really need position from scene node, 
    // but it's needed for receiving messages.
    if (!HasSceneNodeParent())
    {
        DbgWARNING("Light must be attached to scene node!");
        return false;
    }
    m_Type = static_cast<INT32>(LightType::DirectionalLight);
    m_Color = color;
    m_Direction = direction;
    return true;
}

bool Light::InitializePoint(const CBColor& color, float radius)
{
    if (!HasSceneNodeParent())
    {
        DbgWARNING("Light must be attached to scene node!");
        return false;
    }
    m_Type = static_cast<INT32>(LightType::PointLight);
    m_Color = color;
    m_Radius = radius;
    return true;
}

// TODO: this might need a callback.
void Light::RegisterLight()
{
    auto msgPtr = Msg_RegisterLight::Create();
    MESSAGE_FROM_PTR(msgPtr, Msg_RegisterLight)->m_pLight = this;
    CBMessaging::PostQueuedMessage(msgPtr, MessageBus::GetEngineBus());
}
