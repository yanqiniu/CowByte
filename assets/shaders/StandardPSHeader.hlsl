#include "StandardPSInput.hlsl"

// Textures.
Texture2D gAlbedoMap   : register(t0);
SamplerState gAlbedoSS : register(s0);

Texture2D gNormalMap   : register(t1);
SamplerState gNormalSS : register(s1);

Texture2D gSpecularMap   : register(t2);
SamplerState gSpecularSS : register(s2);

Texture2DMS<float> gDepthMap   : register(t3);
SamplerState gDepthSS : register(s3);

// Constants
cbuffer CameraWorldPos : register(b0)
{
    float4 m_CamWorldPos;
}

cbuffer PerMatConstant : register(b1)
{
    float4 m_ColorDiffuse;
    float  m_DiffuseStrength;
    float  m_SpecularStrength;
    float  m_Shininess;
}

// Lights
struct Light
{
    float4    m_Position;
    float4    m_Direction;
    float4    m_Color;
    int       m_Type;
    float     m_Radius;
};

cbuffer Light0 : register(b2)
{
    Light m_Light0;
}

cbuffer Light1 : register(b3)
{
    Light m_Light1;
}

cbuffer Light2 : register(b4)
{
    Light m_Light2;
}

cbuffer Light3 : register(b5)
{
    Light m_Light3;
}