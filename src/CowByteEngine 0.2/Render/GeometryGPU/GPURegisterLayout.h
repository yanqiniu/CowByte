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
    PerMat,

    //////////////////////////////////////////////////////////////////////////
    NumConstBuffer
};

static const int g_nMaxLights = 4;
enum GPUConstantsReg_PS
{
    // Pixel Lights:
    Light0,
    Light1,
    Light2,
    Light3
};

enum GPUTextureReg
{
    AlbedoMap,
    NormalMap,
    SpecularMap,

    NumOfTexturesSupported
};

#endif // 
