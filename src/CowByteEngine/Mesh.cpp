#include "Mesh.h"
#include "path.h"
#include "CBFile.h"
#include "CBStringOps.h"


Mesh::Mesh() :
    UObject()
{
}


Mesh::~Mesh()
{
}

// Configure file path and such. This should run before
// LoadContent(), therefore before Initialize().
bool Mesh::ConfigureMesh(const char* meshName)
{
    size_t someSize = sizeof(CBString<64>);
    m_MeshFilePath.Set(Path::MeshFolder);
    m_MeshFilePath.Append(meshName);
    printf("Mesh path set to: [%s]", m_MeshFilePath.Get());
    return true;
}

bool Mesh::Initialize()
{
    LoadContent();
    return true;
}

bool Mesh::LoadContent()
{
    CBFile meshFile(m_MeshFilePath.Get());
    CBString<64> temp;
    while (meshFile.GetNextNonEmptyLine(temp.Get(), 64))
    {
        temp.Strip(CBStringOps::StripMode::ALL);
        
        printf("[%s]\n", temp.Get());
        float tempfloat;
        char *marker = temp.Get();
        while(CBStringOps::GetNextFloat32(marker, tempfloat, ' '))
        {
            printf("  float found [%f]\n", tempfloat);
        }
    }

    CBString<16> temp1;
    char toParse[64] = "     Why dafuq    is * ememy shippp so               op?     \n";
    char* str = toParse;
    CBStringOps::Strip(str, StripMode::NEWLINE);
    while (CBStringOps::GetNextSubstring((char*&)str, temp1.Get(), temp1.Capacity(), ' '))
    {
        printf("sub string [%s]\n", temp1.Get());
    }

    return true;

}
