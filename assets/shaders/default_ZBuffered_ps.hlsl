#include "StandardPSHeader.hlsl"
#include "PhongLighting.hlsl"
// #include "ZBufferTest.hlsl"

bool ZBufferTest(PS_Input input)
{
    float2 DepthMapPosition;
    DepthMapPosition.x = input.depthPosition.x;
    DepthMapPosition.y = input.depthPosition.y;

    float sampledZ = gDepthMap.Sample(gDepthSS, DepthMapPosition);

    return input.depthPosition.z <= sampledZ;
}
float4 main(PS_Input input) : SV_TARGET
{
    float3 newPos = input.position.xyz;
    float2 DepthMapPosition;
    DepthMapPosition.x = newPos.x;
    DepthMapPosition.y = input.depthPosition.y;

    float sampledZ = gDepthMap.Sample(gDepthSS, DepthMapPosition);

    bool passZTest = (input.depthPosition.z <= sampledZ);
    // if(passZTest == false)
    //     return float4(1.0f, 0.0f, 0.0f, 1.0f);

    input.normal = normalize(input.normal);
    input.tangent = normalize(input.tangent);
    input.bitangent = normalize(input.bitangent);

    //input.texcoord = float2((input.texcoord.x + 1.0f) / 2, (input.texcoord.y + 1.0f) / 2);

    float4 sampleColor = gAlbedoMap.Sample(gAlbedoSS, input.texcoord);
    //float4 outColor = float4(sample, 0.0f, 0.0f, 1.0f);
    //outColor *= saturate(PhongLighting(m_Light0, input) + PhongLighting(m_Light1, input));// + PhongLighting(m_Light2, input) + PhongLighting(m_Light3, input));
    return float4(input.position.x, 0.0f, 0.0f, 1.0f);//sampleColor;//ZBufferTest(input);
}