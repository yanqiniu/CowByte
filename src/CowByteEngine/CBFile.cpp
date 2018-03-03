#include <assert.h>
#include "CBFile.h"
#include "CBStringOps.h"

CBFile::CBFile(const char *fileName)
{
    m_pFile = fopen(fileName, "rt");

    if (!m_pFile)
    {
        // Logger::
    }

    assert(m_pFile != nullptr);
}

CBFile::~CBFile()
{
    fclose(m_pFile);
}

void CBFile::ReadIntoBuffer(char *&buffer, size_t &size)
{
    // grab size of the file first:
    if (fseek(m_pFile, 0, SEEK_END) != 0)
        return;
    size = ftell(m_pFile);

    fseek(m_pFile, 0, SEEK_SET);

    buffer = (char *)malloc(size * 2); // allow space for "//n"

    size_t read = fread(buffer, 1, size, m_pFile);
    buffer[read++] = '\n';
    size = read;
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
