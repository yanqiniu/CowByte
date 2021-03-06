#ifndef _CBSTRINGOPS_H
#define _CBSTRINGOPS_H

#include <stdio.h>
#include <cstring>
#include <assert.h>
#include <cstdlib>

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
                    *(str - numOfSpaces) = *str;
                }
                ++str;
            }
            *(str - numOfSpaces) = *str;
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
            --str;
            while (*str == ' ')
            {
                --str;
            }
            *(str + 1) = '\0';
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
        // skip extra white spaces.
        while (*str == ' ')
        {
            ++str;
        }

        if (strlen(str) == 0)
            return false;

        for (size_t i = 0; i < strlen(str) + 1; ++i)
        {
            if (str[i] == seperators || str[i] == '\0')
            {
                assert(i + 1 <= bufSize);
                memcpy(buf, str, i + 1);
                buf[i] = '\0';

                // advance str pointer
                str = (str[i] == '\0' ? &str[i] : &str[i + 1]);
                return true;
                break;
            }
        }

        return false;
    }

    // converts the next substring into a float. Does not error check
    // to see if it can convert to a float.
    inline bool GetNextFloat32(char *&str, float &buf, char seperator)
    {
        char strbuf[64];
        if (!GetNextSubstring(str, strbuf, 64, ' '))
            return false;
        buf = static_cast<float>(atof(strbuf));
        return true;
    }

    // converts the next substring into a int. Does not error check
    // to see if it can convert to a int.
    inline bool GetNextInt32(char *&str, int &buf, char seperator)
    {
        char strbuf[64];
        if (!GetNextSubstring(str, strbuf, 64, ' '))
            return false;
        buf = atoi(strbuf);
        return true;
    }

    static wchar_t* CharToWChar(const char* text)
    {
        const size_t size = strlen(text) + 1;
        wchar_t* wText = new wchar_t[size];
        mbstowcs(wText, text, size);
        return wText;
    }

}


#endif // !_STRINGOPS_H
