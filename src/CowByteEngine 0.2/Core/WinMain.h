#ifndef _WIN_MAIN_H
#define _WIN_MAIN_H

#ifndef _WINDOW_
    #include <windows.h>
#endif

int _tmain(int argc, char* argv); // For debugging.
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

#endif