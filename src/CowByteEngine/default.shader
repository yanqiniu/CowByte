struct VS_Output
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_Output VShader(float4 position : POSITION, float4 color : COLOR)
{
    VS_Output output;

    output.position = position;
    output.color = color;

    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}