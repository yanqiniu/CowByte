#include "StandardPSHeader.hlsl"
#include "PhongLighting.hlsl"

float4 PShader(PS_Input input) : SV_TARGET
{
    float4 outColor = gAlbedoMap.Sample(gAlbedoSS, input.texcoord) * m_ColorDiffuse;
    outColor *= saturate(PhongLighting(m_Light1, input) + PhongLighting(m_Light1, input));
    return outColor;
}