struct PS_Input
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float4 color : COLOR;
};

float4 PShader(PS_Input input) : SV_TARGET
{
	float4 outColor = input.color;
	float coef = saturate(dot(input.normal, float4( -0.577350f, 0.577350f, -0.577350f, 0.0f)));
	outColor *= coef;
    return outColor;
}