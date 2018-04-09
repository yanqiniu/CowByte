bool ZBufferTest(PS_Input input)
{
    float2 DepthMapPosition;
    DepthMapPosition.x = input.depthPosition.x;
    DepthMapPosition.y = input.depthPosition.y;

    float sampledZ = gDepthMap.Sample(gDepthSS, DepthMapPosition);

    return input.depthPosition.z <= sampledZ;
}