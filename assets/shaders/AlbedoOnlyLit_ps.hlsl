#include "StandardPSHeader.hlsl"
#include "PhongLighting.hlsl"

float4 PShader(PS_Input input) : SV_TARGET
{
    input.normal = normalize(input.normal);
    input.tangent = normalize(input.tangent);
    input.bitangent = normalize(input.bitangent);

    float4 outColor = gAlbedoMap.Sample(gAlbedoSS, input.texcoord) * m_ColorDiffuse;
    outColor *= saturate(PhongLighting(m_Light0, input) + PhongLighting(m_Light1, input));// + PhongLighting(m_Light2, input) + PhongLighting(m_Light3, input));
    return outColor;
}