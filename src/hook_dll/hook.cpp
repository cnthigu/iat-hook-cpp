#include "hook.h"

#include "iat_hook.h"
#include "module_handle.h"

#include <stdio.h>
#include <windows.h>

typedef int ( WINAPI* MessageBoxWFunction ) ( HWND, LPCWSTR, LPCWSTR, UINT );

static MessageBoxWFunction original_message_box_w = NULL;

static int WINAPI MessageBoxWHook ( HWND window, LPCWSTR text, LPCWSTR caption, UINT type )
{
    int result = original_message_box_w ( window, L"Texto MODIFICADO pelo hook!:)", L"Hooked!", MB_OK );

    if ( result == 0 )
    {
        printf ( "[-] original retornou 0 (GetLastError=%lu)\n", GetLastError () );
    }
    else
    {
        printf ( "[+] original retornou: %d\n", result );
    }

    return result;
}

void RunHook ()
{
    printf ( "[+] Obtendo base address do modulo alvo (proprio .exe)...\n" );
    PBYTE target = (PBYTE)GetModuleHandleReplacement ( NULL );
    printf ( "[+] base address = %p\n", target );

    original_message_box_w = (MessageBoxWFunction)HookIAT ( target, "USER32.dll", "MessageBoxW", MessageBoxWHook );

    if ( original_message_box_w == NULL )
    {
        printf ( "[+] [FALHA] nao foi possivel hookar\n" );
    }
    else
    {
        printf ( "[+] [OK] hook aplicado com sucesso\n" );
        printf ( "[+] endereco original guardado: %p\n", original_message_box_w );
    }
}
