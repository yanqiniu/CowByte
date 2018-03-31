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
        float4 diffuse = saturate(dot(input.normal, -light.m_Direction));
        float4 reflected = light.m_Direction - 2 * dot(light.m_Direction, input.normal) * input.normal;
        float4 cameraDir = normalize(m_CamWorldPos - input.worldPos);
        float4 specular = pow(saturate(dot(reflected, cameraDir)), 8);

        return diffuse + specular;
    }
    else if(light.m_Type == 2) // Point
    {
        float4 lightDir = normalize(input.worldPos - light.m_Position);
        float4 diffuse = saturate(dot(input.normal, -lightDir));
        float disCoef = distance(input.worldPos, light.m_Position);
        disCoef = smoothstep(0, light.m_Radius, disCoef);
        disCoef = 1.0f - disCoef;
        diffuse *= disCoef;


        float4 reflected = reflect(lightDir, input.normal);
        float4 cameraDir = normalize(input.worldPos - m_CamWorldPos);
        float specular = pow(saturate(dot(-reflected, cameraDir)), 20) * disCoef;
        
        return (specular + diffuse) * light.m_Color;
    }
    else // Invalid, including -1
    {
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }

}


float4 PShader(PS_Input input) : SV_TARGET
{
    float4 outColor = gAlbedoMap.Sample(gAlbedoSS, input.texcoord);
    outColor *= saturate(Lighting(m_Light1, input) + Lighting(m_Light1, input));
    return outColor;
}