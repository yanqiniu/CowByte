#include "StandardVSInput.hlsl"
#include "StandardPSInput.hlsl"

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

