struct VS_Input
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};


struct PS_Input
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 texcoord : TEXCOORD;
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

PS_Input VShader(VS_Input input)
{
    PS_Input output;
    
    float4x4 mvp = mul(mul(worldMatrix, cameraViewMatrix), projectionMatrix);
    output.position = mul(input.position, mvp );
    output.normal = mul(input.normal, worldMatrix); // transform normal and normalize
    //output.color = input.color;
    output.texcoord = input.texcoord;

    return output;
}
