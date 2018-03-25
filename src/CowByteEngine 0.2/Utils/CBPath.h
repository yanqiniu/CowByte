#ifndef _CBPATH_H
#define _CBPATH_H

#include "CBString.h"
#include "typedefs.h"

namespace CBPath
{
    static const char _assetsFolder[256] = "assets\\";
    static const char _meshFolder[256] = "assets\\meshes\\";
    static const char _textureFolder[256] = "assets\\textures\\";

    static Filepath g_BuildDir;

    // className: mesh, shader...etc.
    void GenerateAssetPath(Filepath &buf, const char *className, const char *fileName);
    void SetWorkingDirectory();
}




#endif // 
