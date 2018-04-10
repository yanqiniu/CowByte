#ifndef _GPU_REGISTER_LAYOUT_H
#define _GPU_REGISTER_LAYOUT_H

enum GPUConstantsReg_VS
{
    //////////////////////////////////////////////////////////////////////////
    // Per applications:


    //////////////////////////////////////////////////////////////////////////
    // Per Frame:
    ViewProjMatrix,
    LightViewProjMatrix,

    //////////////////////////////////////////////////////////////////////////
    // Per Object:
    ObjectWorldMatrix,


    //////////////////////////////////////////////////////////////////////////
    // Per Mat:

    //////////////////////////////////////////////////////////////////////////
    NumConstBuffer_VS
};

static const int g_nMaxLights = 4;
enum GPUConstantsReg_PS
{
    CameraWorldPos,
    PerMat,

    // Pixel Lights:
    Light0,
    Light1,
    Light2,
    Light3,

    NumConstBuffer_PS
};

enum GPUTextureReg
{
    AlbedoMap,
    NormalMap,
    SpecularMap,

    DepthMap,
    ShadowMap, //Used only for one single directional light

    NumOfTexturesSupported
};

enum GPUSamplerStateReg
{
    AlbedoMapSS,
    NormalMapSS,
    SpecularMapSS,
    Point,

    NumOfSamplerStatesSupporteds
};

#endif // 
