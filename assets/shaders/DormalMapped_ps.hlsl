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
        toRet = saturate(dot(input.normal, -toRet));
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
    float4 outColor = gAlbedoMap.Sample(gAlbedoSS, input.texcoord);
    float4 normalSampled = gNormalMap.Sample(gNormalSS, input.texcoord);
    outColor *= saturate(Lighting(m_Light0, input) + Lighting(m_Light1, input));
    return outColor;
}