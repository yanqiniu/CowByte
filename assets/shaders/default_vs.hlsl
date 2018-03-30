#include "StandardVSHeader.hlsl"

PS_Input VShader(VS_Input input)
{
    PS_Input output;
    
    float4x4 mvp = mul(worldMatrix, viewProjectionMatrix);
    output.position = mul(input.position, mvp);
    output.worldPos = mul(input.position, worldMatrix);
    output.texcoord = input.texcoord;

    output.normal = normalize(mul(input.normal, worldMatrix));
    output.tangent = normalize(mul(input.tangent, worldMatrix));
    output.bitangent = normalize(float4(-cross(output.normal, input.tangent), 0.0f));
    

    return output;
}
