// Interpolated vertex attributes.
struct PS_Input
{
    float4 position : SV_POSITION;
    float4 worldPos : POSITION;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float4 bitangent : BITANGENT;
    float2 texcoord : TEXCOORD;
};

// Textures.
Texture2D gAlbedoMap : register(t0);
SamplerState gAlbedoSS : register(s0);

Texture2D gNormalMap : register(t1);
SamplerState gNormalSS : register(s1);

Texture2D gSpecularMap : register(t2);
SamplerState gSpecularSS : register(s2);

// Constants
cbuffer CameraWorldPos : register(b0)
{
    float4x4 m_CamWorldPos;
}

cbuffer PerMatConstant : register(b1)
{
    float4 m_ColorDiffuse;
    float4 m_ColorSpecular;
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