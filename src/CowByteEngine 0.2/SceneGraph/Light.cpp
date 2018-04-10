#include "Light.h"
#include "../Utils/CBDebug.h"
#include "../Messaging/CBMessaging.h"
#include "SceneNode.h"


Light::LightData::LightData() :
    m_Color(1.0f, 1.0f, 1.0f, 1.0f), // Default to white
    m_Position(0.0f, 0.0f, 0.0f, 1.0f),
    m_Direction(0.0f, -1.0f, 0.0f, 0.0f),
    m_Radius(1.0f),
    m_Type(-1),
    m_bHasShadow(false)
{

}


Light::Light() :
    m_Data()
{
}


Light::~Light()
{
}

bool Light::Update(const GameContext &context)
{
    if (m_Data.m_Type == LightType::PointLight)
    {
        if (!HasSceneNodeParent())
        {
            DbgWARNING("Point light must be attached to scene node for position update!");
        }
        else
        {
            m_Data.m_Position = GetParentSceneNode()->GetWorldTransform().GetPosition();
        }
    }
    else if (m_Data.m_Type == LightType::DirectionalLight)
    {
        if (!HasSceneNodeParent())
        {
            DbgWARNING("Directional light must be attached to scene node for rotation update!");
        }
        else
        {
            m_Data.m_Direction = GetParentSceneNode()->GetWorldTransform().Front();
        }
    }

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
    m_Data.m_Type = static_cast<INT32>(LightType::AmbientLight);
    m_Data.m_Color = color;
    return true;
}

bool Light::InitializeDirectional(const CBColor& color, bool hasShadow)
{
    // Yes, directional light doesn't really need position from scene node, 
    // but it's needed for receiving messages.
    if (!HasSceneNodeParent())
    {
        DbgWARNING("Light must be attached to scene node!");
        return false;
    }
    m_Data.m_Type = static_cast<INT32>(LightType::DirectionalLight);
    m_Data.m_Color = color;
    m_Data.m_bHasShadow = hasShadow;
    return true;
}

bool Light::InitializePoint(const CBColor& color, float radius)
{
    if (!HasSceneNodeParent())
    {
        DbgWARNING("Light must be attached to scene node!");
        return false;
    }
    m_Data.m_Type = static_cast<INT32>(LightType::PointLight);
    m_Data.m_Color = color;
    m_Data.m_Radius = radius;
    return true;
}

// TODO: this might need a callback.
void Light::RegisterLight()
{
    auto msgPtr = Msg_RegisterLight::Create();
    MESSAGE_FROM_PTR(msgPtr, Msg_RegisterLight)->m_pLight = this;
    CBMessaging::PostQueuedMessage(msgPtr, MessageBus::GetEngineBus());
}
