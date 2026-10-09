#pragma once

#include <windows.h>

// api_name is either a name or an ordinal (value <= 0xFFFF)
FARPROC GetProcAddressReplacement ( HMODULE module, LPCSTR api_name );
