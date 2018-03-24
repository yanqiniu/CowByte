struct PS_Input
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

Texture2D gTexture : register(t0);
SamplerState gSamplerState : register(s0);

float4 PShader(PS_Input input) : SV_TARGET
{
    float4 outColor = gTexture.Sample(gSamplerState, input.texcoord);
	float coef = saturate(dot(input.normal, float4( -0.577350f, 0.577350f, -0.577350f, 0.0f)));
    if (coef <= 0)
        coef = 0.3f;
	outColor *= coef;
    return outColor;
}