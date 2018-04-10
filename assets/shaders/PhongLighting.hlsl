// TODO: if statement slow.
float4 PhongLighting(Light light, PS_Input input, bool shadowed)
{
    if(shadowed)
    {
        // input.depthPosition /= input.depthPosition.w;
        float2 coord = float2((input.lightViewPosition.x + 1.0f) / 2,
                                    1.0f-(input.lightViewPosition.y + 1.0f) / 2);

        float sampledZ = gShadowMap.Sample(gPointSS, coord);

        if(input.lightViewPosition.z - sampledZ > 0.02) // threshhold for MSAA
            return float4(1.0f, 0.0f, 0.0f, 1.0f);
    }

    if(light.m_Type == 0) // Ambient
    {
        return light.m_Color;
    }
    else if(light.m_Type == 1) // Directional
    {
        float4 diffuse = saturate(dot(input.normal, -light.m_Direction));
        float4 reflected = light.m_Direction - 2 * dot(light.m_Direction, input.normal) * input.normal;
        float4 cameraDir = normalize(m_CamWorldPos - input.worldPos);
        float4 specular = pow(saturate(dot(reflected, cameraDir)), m_Shininess);

        return diffuse * m_DiffuseStrength + specular *  m_SpecularStrength;
    }
    else if(light.m_Type == 2) // Point
    {
        // Diffuse
        float4 lightDir = normalize(input.worldPos - light.m_Position);
        float4 diffuse = saturate(dot(input.normal, -lightDir));

        // Specular
        float4 reflected = reflect(lightDir, input.normal);
        float4 cameraDir = normalize(input.worldPos - m_CamWorldPos);
        float specular = pow(saturate(dot(-reflected, cameraDir)), m_Shininess);

        // Distance
        float disCoef = distance(input.worldPos, light.m_Position);
        disCoef = smoothstep(0, light.m_Radius, disCoef);
        disCoef = 1.0f - disCoef;

        return (specular * m_SpecularStrength + diffuse * m_DiffuseStrength) * disCoef * light.m_Color;
    }
    else // Invalid, including -1
    {
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }

}