#include "StandardPSHeader.hlsl"


float4 PShader(PS_Input input) : SV_TARGET
{
    float4 outColor = gTexture.Sample(gSamplerState, input.texcoord);
	float coef = saturate(dot(input.normal, m_Direction_0));
	outColor *= coef;
    return outColor;
}