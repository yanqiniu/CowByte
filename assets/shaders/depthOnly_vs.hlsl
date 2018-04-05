#include "StandardVSInput.hlsl" // Use default so we don't have to create a seperate vertex layout in C++

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

struct PSInput
{
    float4 position      : SV_POSITION;
    float4 depthPosition : TEXTURE0;
};

PSInput main(VS_Input input)
{
    PSInput output;
    
    float4x4 mvp = mul(worldMatrix, viewProjectionMatrix);
    output.position = mul(input.position, mvp);
    output.depthPosition = float4(output.position.x, output.position.y, output.position.z, 1.0f);
    return output;
}