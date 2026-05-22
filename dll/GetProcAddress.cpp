#include "GetProcAddress.h"

FARPROC GetProcAdressReplacement(IN HMODULE hModule, IN LPCSTR lpApiName)
{
    PBYTE pBase = (PBYTE)hModule;

    PIMAGE_DOS_HEADER pImgDosHdr = (PIMAGE_DOS_HEADER)pBase;
    if (pImgDosHdr->e_magic != IMAGE_DOS_SIGNATURE)
        return NULL;

    PIMAGE_NT_HEADERS pImgNtHdrs = (PIMAGE_NT_HEADERS)(pBase + pImgDosHdr->e_lfanew);
    if (pImgNtHdrs->Signature != IMAGE_NT_SIGNATURE)
        return NULL;

    IMAGE_OPTIONAL_HEADER ImgOpHdr = pImgNtHdrs->OptionalHeader;
    IMAGE_DATA_DIRECTORY exportDataDir = ImgOpHdr.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

    PIMAGE_EXPORT_DIRECTORY pImgExportDir = (PIMAGE_EXPORT_DIRECTORY)(pBase + exportDataDir.VirtualAddress);

    PDWORD FunctionAddressArray = (PDWORD)(pBase + pImgExportDir->AddressOfFunctions);

    PVOID pFunctionAddress = NULL;

    // Busca por ordinal (quando lpApiName cabe em 16 bits)
    if ((ULONG_PTR)lpApiName <= 0xFFFF)
    {
        WORD ordinal = (WORD)((ULONG_PTR)lpApiName & 0xFFFF);
        DWORD base = pImgExportDir->Base;

        if (ordinal < base || ordinal >= base + pImgExportDir->NumberOfFunctions)
            return NULL;

        pFunctionAddress = (PVOID)(pBase + FunctionAddressArray[ordinal - base]);
    }
    // Busca por nome
    else
    {
        PDWORD FunctionNameArray = (PDWORD)(pBase + pImgExportDir->AddressOfNames);
        PWORD  FunctionOrdinalArray = (PWORD)(pBase + pImgExportDir->AddressOfNameOrdinals);

        for (DWORD i = 0; i < pImgExportDir->NumberOfNames; i++)
        {
            CHAR* pFunctionName = (CHAR*)(pBase + FunctionNameArray[i]);
            if (strcmp(lpApiName, pFunctionName) == 0)
            {
                pFunctionAddress = (PVOID)(pBase + FunctionAddressArray[FunctionOrdinalArray[i]]);
                break;
            }
        }
    }

    return (FARPROC)pFunctionAddress;
}