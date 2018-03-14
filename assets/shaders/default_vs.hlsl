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
    
    matrix mvp = mul(worldMatrix, cameraViewMatrix);
    mvp = mul(mvp, projectionMatrix);
    output.position = mul( position, mvp );

    // float4x4 mvp = mul(projectionMatrix, mul(cameraViewMatrix, worldMatrix));
    // output.position = mul( mvp, position );

    // output.position = position;
    output.color = color;

    return output;
}
