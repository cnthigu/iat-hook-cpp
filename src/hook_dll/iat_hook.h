#pragma once

#include <windows.h>

// Replaces the IAT entry of api_name imported from module_name in the image at target.
// Returns the original function pointer, or NULL when the import is not found.
PVOID HookIAT ( PBYTE target, LPCSTR module_name, LPCSTR api_name, LPCVOID replacement );
