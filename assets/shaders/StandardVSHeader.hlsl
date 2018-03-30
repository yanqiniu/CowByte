struct VS_Input
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float2 texcoord : TEXCOORD;
};


struct PS_Input
{
    float4 position : SV_POSITION;
    float4 worldPos : POSITION;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float4 bitangent : BITANGENT;
    float2 texcoord : TEXCOORD;
};

////////////////////////////////////////////////////////
// Per Frame Constants

cbuffer ViewProjMat : register(b0)
{
    matrix viewProjectionMatrix;
}


////////////////////////////////////////////////////////
// Per Object Constants

cbuffer ObjectWorldMat : register(b1)
{
    matrix worldMatrix;
}

