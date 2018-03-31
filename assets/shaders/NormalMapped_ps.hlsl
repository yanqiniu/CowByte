#include "StandardPSHeader.hlsl"
#include "PhongLighting.hlsl"


float4 PShader(PS_Input input) : SV_TARGET
{
    // Tiling.
    input.texcoord *= 5.0f;

    // Sample Albedo.
    //float4 outColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float4 outColor = gAlbedoMap.Sample(gAlbedoSS, input.texcoord) * m_ColorDiffuse;

    // Sample Normal.
    float4 normalSampled = gNormalMap.Sample(gNormalSS, input.texcoord);
    normalSampled = float4(normalSampled.x * 2 - 1.0f , normalSampled.y * 2 - 1.0f, normalSampled.z * 2, 1.0f);

    // Transform sampled normal to tangent space.
    float4x4 TBN = float4x4(input.tangent, input.normal, input.bitangent,  float4(0.0f, 0.0f, 0.0f, 1.0f));
    normalSampled = mul(normalSampled, TBN); 
    normalSampled = normalize(normalSampled);

    // Apply normal and calculate lighting.
    input.normal = float4(normalSampled.x, normalSampled.z, normalSampled.y, 0.0f); // Switch y and z because we are using B value as Y.
    outColor *= saturate(PhongLighting(m_Light1, input));
    return outColor;
}
// Lighting(m_Light0, input) + 