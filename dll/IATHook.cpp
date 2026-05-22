#include "IAThook.h"
#include "util.h"
#include <string.h>

PVOID HookIAT(PBYTE pTarget, LPCSTR lpModuleName, LPCSTR lpApiName, LPCVOID replacement)
{
    PIMAGE_DOS_HEADER pImgDosHdr = (PIMAGE_DOS_HEADER)pTarget;
    if (pImgDosHdr->e_magic != IMAGE_DOS_SIGNATURE)
        return NULL;

    PIMAGE_NT_HEADERS pImgNtHdrs = (PIMAGE_NT_HEADERS)(pTarget + pImgDosHdr->e_lfanew);
    if (pImgNtHdrs->Signature != IMAGE_NT_SIGNATURE)
        return NULL;

    IMAGE_OPTIONAL_HEADER ImgOpHdr = pImgNtHdrs->OptionalHeader;

    IMAGE_DATA_DIRECTORY impDataDir = ImgOpHdr.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

    PIMAGE_IMPORT_DESCRIPTOR pImportAddressTable = (PIMAGE_IMPORT_DESCRIPTOR)(pTarget + impDataDir.VirtualAddress);

    SIZE_T iatSize = impDataDir.Size / sizeof(IMAGE_IMPORT_DESCRIPTOR);

    for (SIZE_T i = 0; i < iatSize; i++)
    {
        if (pImportAddressTable[i].Name == 0)
            break;

        char* pModuleName = (char*)(pTarget + pImportAddressTable[i].Name);

        if (IsEqualCStr(lpModuleName, pModuleName))
        {
            PVOID original = PatchIATEntry(pTarget, lpApiName, &pImportAddressTable[i], replacement);

            if (original != NULL)
                return original; 
        }
    }

    return NULL;
}

PVOID PatchIATEntry(PBYTE pTarget, PCSTR lpApiName, PIMAGE_IMPORT_DESCRIPTOR pModuleEntry, LPCVOID replacement)
{
    PULONG_PTR originalThunk = (PULONG_PTR)(pTarget + pModuleEntry->OriginalFirstThunk);
    PULONG_PTR thunk = (PULONG_PTR)(pTarget + pModuleEntry->FirstThunk);

    while (*originalThunk != NULL)
    {
        if (IMAGE_SNAP_BY_ORDINAL(*originalThunk))
        {
            originalThunk++;
            thunk++;
            continue;
        }

        PIMAGE_IMPORT_BY_NAME importByName = (PIMAGE_IMPORT_BY_NAME)(pTarget + *originalThunk);

        if (strcmp(importByName->Name, lpApiName) == 0)
        {
            PVOID original = (PVOID)(*thunk);

            DWORD protect = 0;
            VirtualProtect(thunk, sizeof(ULONG_PTR), PAGE_READWRITE, &protect);
            *thunk = (ULONG_PTR)replacement;
            VirtualProtect(thunk, sizeof(ULONG_PTR), protect, &protect);

            return original;
        }

        originalThunk++;
        thunk++;
    }

    return NULL;
}