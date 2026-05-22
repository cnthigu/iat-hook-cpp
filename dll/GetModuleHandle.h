#pragma once
#include <windows.h>
#include "util.h"
#include <winternl.h>  

static BOOL IsModuleNameEqual(LPCSTR ansiName, PUNICODE_STRING wideName);

HMODULE GetModuleHandleReplacement(IN LPCSTR szModuleName);