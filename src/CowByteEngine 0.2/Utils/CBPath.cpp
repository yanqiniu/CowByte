#include "CBPath.h"
#include <Windows.h>

namespace Path
{
    // className: mesh, shader...etc.
    void GenerateAssetPath(Filepath &buf, const char *className, const char *fileName)
    {
        buf.Clear();
        buf.Set(AssetsFolder);
        buf.Append(className);
        buf.Append("\\");
        buf.Append(fileName);
    }

    //move to the executable directory and then the Data folder
    void SetWorkingDirectory()
    {
        char path[4096];
        GetModuleFileName(nullptr, path, 4096);
        int length = lstrlen(path);
        int componentsFound = 0;
        for (int i = length - 1; i >= 0; --i)
        {
            if (path[i] == '\\')
            {
                if (++componentsFound == 4)
                {
                    path[i] = '\0';
                    break;
                }
            }
        }
        // Set working directory at CowByte\ folder.
        bool didSucceed = SetCurrentDirectory(path) != 0;
    }
}
