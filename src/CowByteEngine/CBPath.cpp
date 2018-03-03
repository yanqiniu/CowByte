#include "CBPath.h"

namespace Path
{
    // className: mesh, shader...etc.
    void GenerateAssetPath(CBString<256> &buf, const char *className, const char *fileName)
    {
        buf.Clear();
        buf.Set(AssetsFolder);
        buf.Append(className);
        buf.Append("/");
        buf.Append(fileName);
    }
}
