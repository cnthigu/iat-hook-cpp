#include <stdio.h>
#include <windows.h>

int main ()
{
    printf ( "[+] Carregando a DLL de hook...\n" );

    HMODULE hook_dll = LoadLibraryA ( "hook_dll.dll" );

    if ( !hook_dll )
    {
        printf ( "[-] Falha ao carregar DLL. GetLastError = %lu\n", GetLastError () );
        return 1;
    }

    printf ( "[+] DLL carregada. Chamando MessageBoxW...\n" );

    int result = MessageBoxW ( NULL, L"Texto original da mensagem", L"Titulo original", MB_OK | MB_ICONINFORMATION );

    printf ( "[+] MessageBoxW retornou: %d\n", result );

    FreeLibrary ( hook_dll );
    return 0;
}
