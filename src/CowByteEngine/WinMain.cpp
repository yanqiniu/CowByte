#include "WinMain.h"
#include "iostream.h"
#include "string.h"
#include "deletemacros.h"
#include "Engine.h"
#include "Debug.h"
#include "CBMemory.h"

#if defined (DEBUG) | defined(_DEBUG)
int _tmain(int argc, char* argv) 
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    DbgINFO("Starting the program...");

    // Run winmain function
    WinMain((HINSTANCE)GetModuleHandle(NULL), 0, 0, SW_SHOW);

    return 0;
}

#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

#if defined (DEBUG) | defined (_DEBUG)
    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    //Enable run-time memory leak for debug builds.
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetBreakAlloc(0);
#endif

    // create the engine
    Engine *pEngine = new Engine();

    //kick of the game
    int result = pEngine->RunLoop();

    // delete the engine
    SafeDelete(pEngine);

    return 0;
}

