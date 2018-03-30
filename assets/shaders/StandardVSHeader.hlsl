#include "StandardVSInput.hlsl"
#include "StandardPSInput.hlsl"

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

