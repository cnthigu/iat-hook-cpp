#pragma once

#include <windows.h>

// Returns the first loaded module (the host exe) when module_name is NULL
HMODULE GetModuleHandleReplacement ( LPCSTR module_name );
