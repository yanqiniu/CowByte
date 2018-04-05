float4 ZBufferTest(PS_Input input)
{
    float2 DepthMapPosition;
    DepthMapPosition.x = input.depthPosition.x;
    DepthMapPosition.y = input.depthPosition.y;

    float sample = gDepthMap.Sample(gDepthSS, DepthMapPosition);

    return float4(sample, 0.0f, 0.0f, 1.0f);
}