#include <assert.h>
#include "CBFile.h"
#include "CBStringOps.h"
#include "CBDebug.h"
#include "../Memory/CBMemArena.h"

CBFile::CBFile(const char *fileName)
{
    m_pFile = fopen(fileName, "rt");

    if (!m_pFile)
    {
        DbgERROR("Unable to open file: %s", fileName);
    }

    assert(m_pFile != nullptr);
}

CBFile::~CBFile()
{
    fclose(m_pFile);
}

char* CBFile::ReadIntoBuffer(size_t &size)
{
    // grab size of the file first:
    if (fseek(m_pFile, 0, SEEK_END) != 0)
        return nullptr;
    size = ftell(m_pFile);

    fseek(m_pFile, 0, SEEK_SET);

    char* toRet = static_cast<char*>(CBMemArena::Get().Allocate(size * 2)); // allow space for "//n" 

    size_t read = fread(toRet, 1, size, m_pFile);
    toRet[read++] = '\n';
    size = read;

    return toRet;
}

bool CBFile::GetNextNonEmptyLine(char *buffer, size_t maxLineSize, bool doStripNewLine)
{
    while (fgets(buffer, maxLineSize, m_pFile) != NULL)
    {
        bool isEmpty = true;
        // scan the line to see if it's empty.
        for (size_t i = 0; i < strlen(buffer); ++i)
        {
            if (buffer[i] != ' ' && buffer[i] != '\n' && buffer[i] != '\0')
            {
                //check if starts with #, means it's a comment.
                if (buffer[i] != '#')
                {
                    isEmpty = false;
                }
                break;
            }
        }

        // Already found a non empty line and buffer filled.
        if (!isEmpty)
        {
            if (doStripNewLine)
                CBStringOps::Strip(buffer, CBStringOps::StripMode::NEWLINE);
            return true;

        }
    }

    return false;
}
