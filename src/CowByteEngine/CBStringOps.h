#ifndef _CBSTRINGOPS_H
#define _CBSTRINGOPS_H

#include <stdio.h>
#include <cstring>

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
    inline bool _removeNewLineSymbolFromEnd(char *cstring)
    {
        if (strlen(cstring) == 0)
            return false;
        else if (cstring[strlen(cstring) - 1] != '\n')
            return true;
        else
        {
            cstring[strlen(cstring) - 1] = '\0';
            return true;
        }
    }

    // This moves the array elements so we don't 
    // run out of spaces later.
    inline bool _removeSpacesFromFront(char *cstring)
    {
        if (strlen(cstring) == 0)
            return false;
        else if (*cstring != ' ')
            return false;
        else
        {
            size_t numOfSpaces = 0;
            bool foundFirstNonSpace = false;
            while (*cstring != '\0')
            {
                if (!foundFirstNonSpace && *cstring == ' ')
                    ++numOfSpaces;
                else
                {
                    foundFirstNonSpace = true;
                    *(cstring - numOfSpaces * sizeof(char)) = *cstring;
                }
                cstring += sizeof(char);
            }
            *(cstring - numOfSpaces * sizeof(char)) = *cstring;
            return true;
        }
    }

    inline bool _removeSpacesFromEnd(char *cstring)
    {
        if (strlen(cstring) == 0)
            return false;
        else if (cstring[strlen(cstring) - 1] != ' ')
            return false;
        else
        {
            char *start = cstring;
            cstring = start + strlen(start);
            cstring -= sizeof(char);
            while (*cstring == ' ')
            {
                cstring -= sizeof(char);
            }
            *(cstring + sizeof(char)) = '\0';
            return true;
        }
    }

    inline bool Strip(char *cstring, StripMode mode)
    {
        bool result = false;

        if (mode == StripMode::NEWLINE ||
            mode == StripMode::ALL)
            result |= CBStringOps::_removeNewLineSymbolFromEnd(cstring);

        if (mode == StripMode::SPACES_FRONT ||
            mode == StripMode::SPACES ||
            mode == StripMode::ALL)
            result |= CBStringOps::_removeSpacesFromFront(cstring);

        if (mode == StripMode::SPACES_BACK ||
            mode == StripMode::SPACES ||
            mode == StripMode::ALL)
            result |= CBStringOps::_removeSpacesFromEnd(cstring);


        return result;
    }
}


#endif // !_STRINGOPS_H
