#include "StandardPSHeader.hlsl"
#include "PhongLighting.hlsl"
// #include "ZBufferTest.hlsl"
float4 main(PS_Input input) : SV_TARGET
{
    // input.depthPosition /= input.depthPosition.w;
    float2 screenCoord = float2((input.depthPosition.x / input.depthPosition.w + 1.0f) / 2,
                                1.0f-(input.depthPosition.y / input.depthPosition.w + 1.0f) / 2);

    float sampledZ = gDepthMap.Sample(gDepthSS, screenCoord);

    if(input.depthPosition.z - sampledZ > 0.02) // threshhold for MSAA
        return float4(0.0f, 1.0f, 0.0f, 1.0f);

    return gAlbedoMap.Sample(gAlbedoSS, input.texcoord);
}