#include <windows.h>
#include <stdio.h>

int main()
{
    printf("[+] Carregando a DLL de hook...\n");

    HMODULE hDll = LoadLibraryA("dll.dll"); 

    if (!hDll)
    {
        printf("[-] Falha ao carregar DLL. GetLastError = %lu\n", GetLastError());
        return 1;
    }

    printf("[+] DLL carregada. Chamando MessageBoxW...\n");

    int result = MessageBoxW(NULL,L"Texto original da mensagem",L"Titulo original",MB_OK | MB_ICONINFORMATION);

    printf("[+] MessageBoxW retornou: %d\n", result);

    FreeLibrary(hDll);
    return 0;
}