// Interpolated vertex attributes.
struct PS_Input
{
    float4 position          : SV_POSITION;
    float4 worldPos          : POSITION0;
    float4 depthPosition     : POSITION1;
    float4 lightViewPosition : POSITION2;
    float4 normal            : NORMAL;
    float4 tangent           : TANGENT;
    float4 bitangent         : BINORMAL;
    float2 texcoord          : TEXCOORD;
};