#if defined(WIN32)
    #include <windows.h>
#endif

#define CONSOLE

#include "game/Runner.hpp"

#ifdef CONSOLE
int main()
#elif WIN32
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpszArgument, int iShow)
#endif
{
    Runner runner;
    try {
        runner.run();
    }
    catch (...) {

    }
    return 0;
}