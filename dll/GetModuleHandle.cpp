#include "GetModuleHandle.h"

static BOOL IsModuleNameEqual(LPCSTR ansiName, PUNICODE_STRING wideName)
{
    if (ansiName == NULL || wideName == NULL || wideName->Buffer == NULL)
        return FALSE;

    USHORT wideLen = wideName->Length / sizeof(WCHAR);

    USHORT i = 0;
    for (; i < wideLen && ansiName[i] != '\0'; i++)
    {
        WCHAR wc = wideName->Buffer[i];
        CHAR  ac = ansiName[i];

        if (wc >= L'A' && wc <= L'Z') wc += 32;
        if (ac >= 'A' && ac <= 'Z')  ac += 32;

        if ((WCHAR)ac != wc)
            return FALSE;
    }

    return (i == wideLen && ansiName[i] == '\0');
}

HMODULE GetModuleHandleReplacement(IN LPCSTR szModuleName)
{
  
#ifdef _WIN64
    PPEB pPeb = (PPEB)__readgsqword(0x60);
#else
    PPEB pPeb = (PPEB)__readfsdword(0x30);
#endif

    PPEB_LDR_DATA pLdr = pPeb->Ldr;

    PLIST_ENTRY pHead = &pLdr->InMemoryOrderModuleList;
    PLIST_ENTRY pCurrent = pHead->Flink;

    while (pCurrent != pHead)
    {
        PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD(pCurrent, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

        if (szModuleName == NULL)
        {
            return (HMODULE)pEntry->DllBase;
        }

        if (IsModuleNameEqual(szModuleName, (PUNICODE_STRING)&pEntry->Reserved4))
        {
            return (HMODULE)pEntry->DllBase;
        }

        pCurrent = pCurrent->Flink;
    }

    return NULL;
}