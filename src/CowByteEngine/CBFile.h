#ifndef _CBFILE_H
#define _CBFILE_H

#include <iostream>
#include <fstream>
#include <stdio.h>

// CowByte engine text file reader.
class CBFile
{
public:
    explicit CBFile(const char *fileName);
    ~CBFile();

    void ReadIntoBuffer(char *&buffer, size_t &size);
    bool GetNextNonEmptyLine(char *buffer, size_t maxLineSize);

private:
    FILE *m_pFile;
};

#endif // define _CBFILE_H



