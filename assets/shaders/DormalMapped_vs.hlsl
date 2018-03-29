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

PS_Input VShader(VS_Input input)
{
    PS_Input output;
    
    float4x4 mvp = mul(worldMatrix, viewProjectionMatrix);
    output.position = mul(input.position, mvp);
    output.normal = normalize(mul(input.normal, worldMatrix)); // transform normal and normalize
    output.texcoord = input.texcoord;

    return output;
}
