// Interpolated vertex attributes.
struct PS_Input
{
    float4 position  : SV_POSITION;
    float4 worldPos  : POSITION;
    float4 depthPosition : TEXTURE0;
    float4 normal    : NORMAL;
    float4 tangent   : TANGENT;
    float4 bitangent : BINORMAL;
    float2 texcoord  : TEXCOORD;
};