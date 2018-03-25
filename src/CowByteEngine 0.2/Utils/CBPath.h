#ifndef _CBPATH_H
#define _CBPATH_H

#include "CBString.h"
#include "typedefs.h"

namespace CBPath
{
    static const char AssetsFolder[256] = "assets\\";
    static const char MeshFolder[256] = "assets\\meshes\\";
    static const char TextureFolder[256] = "assets\\textures\\";

    // className: mesh, shader...etc.
    void GenerateAssetPath(Filepath &buf, const char *className, const char *fileName);
    void SetWorkingDirectory();
}




#endif // 
