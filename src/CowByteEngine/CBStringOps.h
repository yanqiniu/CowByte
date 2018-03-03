#ifndef _CBSTRINGOPS_H
#define _CBSTRINGOPS_H

#include <stdio.h>
#include <cstring>
#include <assert.h>

// string operations, mostly on C-strings.
namespace CBStringOps
{
    enum StripMode
    {
        SPACES_FRONT = 0,
        SPACES_BACK = 1,
        NEWLINE = 2,
        SPACES = 3,
        ALL = 4
    };

    // Remove \n from end of line.
    inline bool _removeNewLineSymbolFromEnd(char *str)
    {
        if (strlen(str) == 0)
            return false;
        else if (str[strlen(str) - 1] != '\n')
            return true;
        else
        {
            str[strlen(str) - 1] = '\0';
            return true;
        }
    }

    // This moves the array elements so we don't 
    // run out of spaces later.
    inline bool _removeSpacesFromFront(char *str)
    {
        if (strlen(str) == 0)
            return false;
        else if (*str != ' ')
            return false;
        else
        {
            size_t numOfSpaces = 0;
            bool foundFirstNonSpace = false;
            while (*str != '\0')
            {
                if (!foundFirstNonSpace && *str == ' ')
                    ++numOfSpaces;
                else
                {
                    foundFirstNonSpace = true;
                    *(str - numOfSpaces * sizeof(char)) = *str;
                }
                str += sizeof(char);
            }
            *(str - numOfSpaces * sizeof(char)) = *str;
            return true;
        }
    }

    inline bool _removeSpacesFromEnd(char *str)
    {
        if (strlen(str) == 0)
            return false;
        else if (str[strlen(str) - 1] != ' ')
            return false;
        else
        {
            char *start = str;
            str = start + strlen(start);
            str -= sizeof(char);
            while (*str == ' ')
            {
                str -= sizeof(char);
            }
            *(str + sizeof(char)) = '\0';
            return true;
        }
    }

    inline bool Strip(char *str, StripMode mode)
    {
        bool result = false;

        if (mode == StripMode::NEWLINE ||
            mode == StripMode::ALL)
            result |= CBStringOps::_removeNewLineSymbolFromEnd(str);

        if (mode == StripMode::SPACES_FRONT ||
            mode == StripMode::SPACES ||
            mode == StripMode::ALL)
            result |= CBStringOps::_removeSpacesFromFront(str);

        if (mode == StripMode::SPACES_BACK ||
            mode == StripMode::SPACES ||
            mode == StripMode::ALL)
            result |= CBStringOps::_removeSpacesFromEnd(str);


        return result;
    }

    // This assumes that string has been cleanly stripped.
    inline bool GetNextSubstring(char *&str, char *buf, size_t bufSize, char seperators)
    {
        if (strlen(str) == 0)
            return false;

        bool found = false;
        for (size_t i = 0; i < strlen(str) + 1; ++i)
        {
            if (str[i] == seperators || str[i] == '\0')
            {
                assert((i + 1) * sizeof(char) <= bufSize);
                memcpy(buf, str, (i + 1) * sizeof(char));
                buf[i] = '\0';

                // advance str pointer
                str = (str[i] == '\0' ? &str[i] : &str[i + 1]);
                found = true;
                break;
            }
        }

        // skip extra white spaces.
        while (*str == ' ')
        {
            str += sizeof(char);
        }

        return found;
    }

    inline float GetNextFloat32(char *&str, char seperator)
    {
        return 0;
        // This 
    }
}


#endif // !_STRINGOPS_H
