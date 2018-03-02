#ifndef _PATH_H
#define _PATH_H

#include "string.h"
#define _CRT_SECURE_NO_WARNINGS

namespace Path
{

#define MAX_PATH_LEN 64

    static const char AssetsFolder[MAX_PATH_LEN] = "../../assets/";

    // TODO: make this configurable with AssetsFolder
    static const char MeshFolder[MAX_PATH_LEN] = "../../assets/meshes/";

    class PathHelper
    {
    public:

        // Return a path formed by two cstrings.
        static const char* ConcatPath(const char* path1, const char* path2)
        {
            size_t len1 = strlen(path1);
            size_t len2 = strlen(path2);
            if (len1 + len2 + 1 > MAX_PATH_LEN)
            {
                // Logger::Log("Concatenated path too long!");
                return nullptr;
            }

            ClearPathBuff();
            //char toRet[MAX_PATH_LEN];
            memcpy(_PathBuf, path1, len1);
            memcpy(_PathBuf + len1, path2, len2);
            _PathBuf[len1 + len2] = '\0';

            char* toRet = _PathBuf;

            return toRet;
        }

    private:
        // This exist so we don't need to do heap allocation.
        static char _PathBuf[MAX_PATH_LEN];
        static void ClearPathBuff()
        {
            memset(_PathBuf, 0, MAX_PATH_LEN);
        }
    };

    // Initialize static member.
    char PathHelper::_PathBuf[MAX_PATH_LEN];
}




#endif // 
