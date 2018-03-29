#include "StandardVSHeader.hlsl"

PS_Input VShader(VS_Input input)
{
    PS_Input output;
    
    float4x4 mvp = mul(worldMatrix, viewProjectionMatrix);
    output.position = mul(input.position, mvp);
    output.normal = normalize(mul(input.normal, worldMatrix)); // transform normal and normalize
    output.texcoord = input.texcoord;

    return output;
}
