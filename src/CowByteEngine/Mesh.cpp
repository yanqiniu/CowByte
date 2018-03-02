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
    return true;

}
