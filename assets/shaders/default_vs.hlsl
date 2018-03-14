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
    
    float4x4 mvp = mul(mul(worldMatrix, cameraViewMatrix), projectionMatrix);
    output.position = mul(position, mvp );
    output.color = color;

    return output;
}
