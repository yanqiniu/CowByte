#include "StandardPSHeader.hlsl"

// TODO: if statement slow.
float4 DiffuseLighting(Light light, PS_Input input)
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

float4 SpecularLighting(Light light, PS_Input input)
{
    if(light.m_Type == 1) // Directional
    {
        float4 reflected = reflect(light.m_Direction, input.normal); //light.m_Direction - 2 * dot(light.m_Direction, input.normal) * input.normal;
        reflected *= (-1.0f);
        float4 cameraDir = normalize(input.worldPos - m_CamWorldPos);
        float4 specular = saturate(dot(reflected, -cameraDir) ) * float4(1.0f, 0.0f, 0.0f, 1.0f); // TODO: this should be a light attribute!
        return specular * 0.1f;
    }
    else if(light.m_Type == 2) // Point
    {
        float dir = normalize(input.worldPos - light.m_Position);
        float4 reflected = reflect(dir, input.normal); //light.m_Direction - 2 * dot(light.m_Direction, input.normal) * input.normal;
        reflected *= (-1.0f);
        float4 cameraDir = normalize(input.worldPos - m_CamWorldPos);
        float4 specular = saturate(dot(reflected, cameraDir) ) * float4(0.0f, 0.0f, 1.0f, 1.0f); // TODO: this should be a light attribute!
        return specular * 0.1f;
    }
    else // Invalid, including -1
    {
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }

}



float4 PShader(PS_Input input) : SV_TARGET
{
    float4 outColor = gAlbedoMap.Sample(gAlbedoSS, input.texcoord);
    outColor *= saturate(DiffuseLighting(m_Light0, input) + DiffuseLighting(m_Light1, input));
    // outColor += SpecularLighting(m_Light0, input);
    outColor += SpecularLighting(m_Light1, input);
    return outColor;
}