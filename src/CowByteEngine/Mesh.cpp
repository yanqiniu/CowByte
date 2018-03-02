#include "Mesh.h"
#include "path.h"
#define _CRT_SECURE_NO_WARNINGS


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
    const char *newPath = Path::PathHelper::ConcatPath(Path::MeshFolder, meshName);
    strcpy_s(m_MeshFilePath, newPath);
    printf("Mesh path set to: [%s]", m_MeshFilePath);
    return true;
}

bool Mesh::Initialize()
{
    LoadContent();
    return true;
}

bool Mesh::LoadContent()
{
    return true;

}
