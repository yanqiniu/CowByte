#ifndef _GPU_REGISTER_LAYOUT_H
#define _GPU_REGISTER_LAYOUT_H

enum GPUConstantsReg_VS
{
    //////////////////////////////////////////////////////////////////////////
    // Per applications:


    //////////////////////////////////////////////////////////////////////////
    // Per Frame:
    ViewProjMatrix,

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

    // One for each light.
    ShadowMap0,
    ShadowMap1,
    ShadowMap2,
    ShadowMap3,

    NumOfTexturesSupported
};

#endif // 
