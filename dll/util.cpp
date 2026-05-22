#include "util.h"
#include <stdio.h>

void EnableDebugConsole()
{
    if (AllocConsole())
    {
        FILE* fpstdout;
        FILE* fpstderr;
        FILE* fpstdin;

        freopen_s(&fpstdout, "CONOUT$", "w", stdout);
        freopen_s(&fpstderr, "CONOUT$", "w", stderr);
        freopen_s(&fpstdin, "CONIN$", "r", stdin);
    }
}

int IsEqualCStr(const char* a, const char* b)
{
    if (!a || !b)
        return 0;

    return _stricmp(a, b) == 0;
}