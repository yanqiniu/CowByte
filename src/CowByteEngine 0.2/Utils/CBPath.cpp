#include <Windows.h>

#include "CBPath.h"
#include "CBStringOps.h"
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

    LPCWSTR GetShaderPath(const char *shaderFileName)
    {
        Filepath temp = g_BuildDir;
        temp.Append(shaderFileName);
        return CBStringOps::CharToWChar(temp.Get());
    }

    //Move to the executable directory and then the Data folder
    void SetWorkingDirectory()
    {
        char path[1024];
        char* buildPath = new char[1024];
        char* marker = buildPath;
        GetModuleFileName(nullptr, path, 1024);
        strcpy(buildPath, path);

        int length = lstrlen(path);
        int componentsFound = 0;
        for (int i = length - 1; i >= 0; --i)
        {
            if (path[i] == '\\')
            {
                ++componentsFound;
                if (componentsFound == 1)
                {
                    buildPath[i] = '\0';
                }
                if (componentsFound == 4)
                {
                    marker = buildPath + i + 1;
                    g_BuildDir.Set(marker);
                    g_BuildDir.Append("\\");
                    path[i] = '\0';
                    break;
                }
            }
        }
        // Set working directory at CowByte\ folder.
        bool didSucceed = SetCurrentDirectory(path) != 0;


        delete buildPath;
    }
}
