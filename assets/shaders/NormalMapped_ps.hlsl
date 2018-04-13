#include "StandardPSHeader.hlsl"
#include "PhongLighting.hlsl"


float4 PShader(PS_Input input) : SV_TARGET
{
    // input.depthPosition /= input.depthPosition.w;
    float2 screenCoord = float2((input.depthPosition.x / input.depthPosition.w + 1.0f) / 2,
                                1.0f-(input.depthPosition.y / input.depthPosition.w + 1.0f) / 2);

    float sampledZ = gDepthMap.Sample(gPointSS, screenCoord);

    if(input.depthPosition.z - sampledZ > 0.02) // threshhold for MSAA
        return float4(0.0f, 0.0f, 0.0f, 1.0f);

    // Tiling.  
    input.texcoord *= 5.0f;

    // Sample Albedo.
    //float4 outColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float4 outColor = gAlbedoMap.Sample(gAlbedoSS, input.texcoord) * m_ColorDiffuse;

    // Sample Normal.
    float3 normalSampled = gNormalMap.Sample(gNormalSS, input.texcoord).xyz;
    normalSampled = 2.0 * normalSampled - 1.0f;

    float3 N = normalize(input.normal.xyz);
    float3 T = normalize(input.tangent.xyz - dot(input.tangent.xyz, N)*N);
    float3 B = cross(T, N);

    // Transform sampled normal to tangent space.
    float3x3 TBN = float3x3(T, B, N);
    normalSampled = normalize(mul(normalSampled, TBN)); 

    // Apply normal and calculate lighting.
    input.normal = float4(normalSampled, 0.0f);//float4(normalSampled.x, normalSampled.z, normalSampled.y, 0.0f); // Switch y and z because we are using B value as Y.
    outColor *= saturate(PhongLighting(m_Light0, input, false));
    return outColor;
}
