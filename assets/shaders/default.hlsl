struct VS_Output
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};


cbuffer PerApplication : register(b0)
{
    matrix projectionMatrix;
}

cbuffer PerFrame : register(b1)
{
    matrix cameraViewMatrix;
}

cbuffer PerObject : register(b2)
{
    matrix worldMatrix;
}

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