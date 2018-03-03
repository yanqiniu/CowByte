#ifndef _CBPATH_H
#define _CBPATH_H

#include "CBString.h"

namespace Path
{
    static const char AssetsFolder[256] = "../../assets/";
    static const char MeshFolder[256] = "../../assets/meshes/";

    // className: mesh, shader...etc.
    void GenerateAssetPath(CBString<256> &buf, const char *className, const char *fileName);
}




#endif // 
