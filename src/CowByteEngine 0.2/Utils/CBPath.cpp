#include "CBPath.h"
#include <Windows.h>

namespace CBPath
{
    // className: mesh, shader...etc.
    void GenerateAssetPath(Filepath &buf, const char *className, const char *fileName)
    {
        buf.Clear();
        buf.Set(_assetsFolder);
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
                ++componentsFound;
                if (componentsFound == 1)
                {
                    // Set build directory:
                    path[i] = '\0';
                    g_BuildDir.Set(path);
                }
                else if (componentsFound == 4)
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
