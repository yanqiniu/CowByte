#include "StandardVSInput.hlsl" // Use default so we don't have to create a seperate vertex layout in C++

////////////////////////////////////////////////////////
// Per Frame Constants

cbuffer ViewProjMat : register(b0)
{
    matrix viewProjectionMatrix;
}

cbuffer LightViewProjMat : register(b1)
{
    matrix lightViewProjectionMatrix;
}

////////////////////////////////////////////////////////
// Per Object Constants

cbuffer ObjectWorldMat : register(b2)
{
    matrix worldMatrix;
}


struct PSInput
{
    float4 position      : SV_POSITION;
    float4  depthPos     : POSITION;
};

PSInput main(VS_Input input)
{
    PSInput output;
    
    float4x4 mlp = mul(worldMatrix, lightViewProjectionMatrix);
    output.position = mul(input.position, mlp);

    output.depthPos = output.position;

    return output;
}