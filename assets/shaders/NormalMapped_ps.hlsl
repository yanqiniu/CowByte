#include "StandardPSHeader.hlsl"

// TODO: if statement slow.
float4 Lighting(Light light, PS_Input input)
{
    if(light.m_Type == 0) // Ambient
    {
        return light.m_Color;
    }
    else if(light.m_Type == 1) // Directional
    {
        return saturate(dot(input.normal, light.m_Direction)) * light.m_Color;
    }
    else if(light.m_Type == 2) // Point
    {
        float4 toRet = normalize(input.worldPos - light.m_Position);
        toRet = dot(input.normal, -toRet);
        float disCoef = distance(input.worldPos, light.m_Position);
        disCoef = smoothstep(0, light.m_Radius, disCoef);
        disCoef = 1.0f - disCoef;
        toRet *= disCoef;
        toRet *= light.m_Color;
        return toRet;
    }
    else // Invalid, including -1
    {
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }

}


float4 PShader(PS_Input input) : SV_TARGET
{
    // Tiling.
    input.texcoord *= 10.0f;

    // Sample Albedo.
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
    outColor *= saturate(Lighting(m_Light1, input));
    return outColor;
}
// Lighting(m_Light0, input) + 