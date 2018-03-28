#ifndef _GPU_REGISTER_LAYOUT_H
#define _GPU_REGISTER_LAYOUT_H

enum GPUConstantsReg
{
    //////////////////////////////////////////////////////////////////////////
    // Per applications:


    //////////////////////////////////////////////////////////////////////////
    // Per Frame:
    ViewProjMatrix,
    // Lights:
    //Light0,
    //Light1,
    //Light2,
    //Light3,

    //////////////////////////////////////////////////////////////////////////
    // Per Object:
    ObjectWorldMatrix,


    //////////////////////////////////////////////////////////////////////////
    // Per Mat:
    PerMat,

    //////////////////////////////////////////////////////////////////////////
    NumConstBuffer
};

enum GPUTextureReg
{
    AlbedoMap,
    NormalMap,
    SpecularMap,

    NumOfTexturesSupported
};

#endif // 
