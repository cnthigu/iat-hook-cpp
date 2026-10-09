#include "windows.h"
#include "hook.h"
#include "util.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason,LPVOID lpReserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        EnableDebugConsole();
        RunHook();
        break;
    case DLL_THREAD_ATTACH:

    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}