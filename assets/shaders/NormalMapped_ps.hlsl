#include "StandardPSHeader.hlsl"
// #include "PhongLighting.hlsl"

// TODO: if statement slow.
float4 PhongLighting(Light light, PS_Input input, bool shadowed)
{
    if(shadowed)
    {
        // input.depthPosition /= input.depthPosition.w;
        float2 coord = float2((input.lightViewPosition.x + 1.0f) / 2,
                                    1.0f-(input.lightViewPosition.y + 1.0f) / 2);
        if(coord.x >= 0 && coord.x <= 1.0f &&
           coord.y >= 0 && coord.y <= 1.0f )
        {
            float sampledZ = gShadowMap.Sample(gPointSS, coord);

            if(input.lightViewPosition.z - sampledZ > 0.02) // threshhold for MSAA
                return float4(0.0f, 0.0f, 0.0f, 1.0f);
        }
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
    outColor *= saturate(PhongLighting(m_Light0, input, true));
    return outColor;
}
