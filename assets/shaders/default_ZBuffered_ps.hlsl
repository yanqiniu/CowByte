#include "StandardPSHeader.hlsl"
#include "PhongLighting.hlsl"
// #include "ZBufferTest.hlsl"
float4 main(PS_Input input) : SV_TARGET
{
    // input.depthPosition /= input.depthPosition.w;
    float2 screenCoord = float2((input.depthPosition.x / input.depthPosition.w + 1.0f) / 2,
                                 1.0f-(input.depthPosition.y / input.depthPosition.w + 1.0f) / 2);

    float sampledZ = 0;
    sampledZ += gDepthMap.Load(input.position.xy, 0);
    sampledZ += gDepthMap.Load(input.position.xy, 1);
    sampledZ += gDepthMap.Load(input.position.xy, 2);
    sampledZ += gDepthMap.Load(input.position.xy, 3);
    sampledZ /= 4;

    if(input.depthPosition.z > sampledZ)
        return float4(0.0f, sampledZ, 0.0f, 1.0f);

    input.normal = normalize(input.normal);
    input.tangent = normalize(input.tangent);
    input.bitangent = normalize(input.bitangent);

    float4 sampleColor = gAlbedoMap.Sample(gAlbedoSS, input.texcoord);
    return sampleColor;
}