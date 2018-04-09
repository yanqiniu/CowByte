struct PSInput
{
    float4 position      : SV_POSITION;
    float4  depthPos     : POSITION;
};


float main(PSInput input) : SV_TARGET
{
    return input.depthPos.z;
}
