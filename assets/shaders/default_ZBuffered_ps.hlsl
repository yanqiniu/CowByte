#include "StandardPSHeader.hlsl"
#include "PhongLighting.hlsl"
//#include "ZBufferTest.hlsl"

float4 main(PS_Input input) : SV_TARGET
{
    input.normal = normalize(input.normal);
    input.tangent = normalize(input.tangent);
    input.bitangent = normalize(input.bitangent);

    //input.texcoord = float2((input.texcoord.x + 1.0f) / 2, (input.texcoord.y + 1.0f) / 2);

    float4 sampleColor = gDepthMap.Sample(gAlbedoSS, input.texcoord);
    //float4 outColor = float4(sample, 0.0f, 0.0f, 1.0f);
    //outColor *= saturate(PhongLighting(m_Light0, input) + PhongLighting(m_Light1, input));// + PhongLighting(m_Light2, input) + PhongLighting(m_Light3, input));
    return sampleColor;//ZBufferTest(input);
}