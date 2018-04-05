#include "StandardVSHeader.hlsl"

PS_Input VShader(VS_Input input)
{
    PS_Input output;
    
    float4x4 mvp = mul(worldMatrix, viewProjectionMatrix);
    output.position = mul(input.position, mvp);
    output.worldPos = mul(input.position, worldMatrix);
    output.depthPosition = float4(output.position.x, output.position.y, output.position.z, 1.0f);
    output.texcoord = input.texcoord;

    output.normal = mul(input.normal, worldMatrix);
    output.tangent = mul(input.tangent, worldMatrix);
    output.bitangent = float4(-cross(output.normal, input.tangent), 0.0f);
    

    return output;
}
