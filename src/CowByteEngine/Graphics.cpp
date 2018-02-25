#include <exception>

#include "Graphics.h"
#include "exceptions.h"

GraphicsData::GraphicsData()
{

}


Graphics::Graphics()
{

}

Graphics::~Graphics()
{
}

bool Graphics::Initialize()
{
    System::Initialize();
    return true;
}

bool Graphics::Update(Context& context)
{
    return true;
}

bool Graphics::ShutDown()
{
    return true;
}


