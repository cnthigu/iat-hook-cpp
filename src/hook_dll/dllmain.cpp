#include "hook.h"
#include "util.h"

#include <windows.h>

BOOL APIENTRY DllMain ( HMODULE module, DWORD reason, LPVOID reserved )
{
    if ( reason == DLL_PROCESS_ATTACH )
    {
        EnableDebugConsole ();
        RunHook ();
    }

    return TRUE;
}
