struct PS_Input
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

Texture2D gTexture : register(t0);
SamplerState gSamplerState : register(s0);

cbuffer Light0 : register(b0)
{
    float4    m_Position_0;
    float4    m_Direction_0;
    float4    m_Color_0;
    int       m_Type_0;
    float     m_Radius_0;
}