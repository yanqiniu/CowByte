#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <windows.h>
#include <windowsx.h>


#include "System.h"
#include "Window.h"


struct GraphicsData : SystemData
{
    explicit GraphicsData();
};

class Graphics : public System
{
public:
    Graphics();
    virtual ~Graphics();
    virtual bool Initialize();
    virtual bool Update(Context& context);
    virtual bool ShutDown();

private:


};

#endif