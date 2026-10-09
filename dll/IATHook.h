#pragma once
#include <windows.h>

PVOID HookIAT(PBYTE pTarget, LPCSTR lpModuleName, LPCSTR lpApiName, LPCVOID replacement);
PVOID PatchIATEntry(PBYTE pTarget, PCSTR lpApiName, PIMAGE_IMPORT_DESCRIPTOR pModuleEntry, LPCVOID replacement);