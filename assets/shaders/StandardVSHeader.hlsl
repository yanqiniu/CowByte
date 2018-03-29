struct VS_Input
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};


struct PS_Input
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};


cbuffer ViewProjMat : register(b0)
{
    matrix viewProjectionMatrix;
}

cbuffer ObjectWorldMat : register(b1)
{
    matrix worldMatrix;
}