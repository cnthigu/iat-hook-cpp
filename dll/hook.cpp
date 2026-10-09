#include "hook.h"
#include "GetModuleHandle.h"
#include "GetProcAddress.h"
#include "IATHook.h"
#include <stdio.h>

typedef int (WINAPI* MessageBoxW_T)(HWND, LPCWSTR, LPCWSTR, UINT);

MessageBoxW_T MessageBoxWOriginal = NULL;

int WINAPI MessageBoxWHook(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
    int result = MessageBoxWOriginal(hWnd, L"Texto MODIFICADO pelo hook!:)", L"Hooked!", MB_OK);

    if (result == 0)
    {
        printf("[-] original retornou 0 (GetLastError=%lu)\n", GetLastError());
    }
    else
    {
        printf("[+] original retornou: %d\n", result);
    }

    return result;
};

void RunHook()
{
    printf("[+] Obtendo base address do modulo alvo (proprio .exe)...\n");
    PBYTE pTarget = (PBYTE)GetModuleHandleReplacement(NULL);
    printf("[+] base address = %p\n", pTarget);

    MessageBoxWOriginal = (MessageBoxW_T)HookIAT(pTarget, "USER32.dll", "MessageBoxW", MessageBoxWHook);

    if (MessageBoxWOriginal == NULL)
    {
        printf("[+] [FALHA] nao foi possivel hookar\n");
    }
    else
    {
        printf("[+] [OK] hook aplicado com sucesso\n");
        printf("[+] endereco original guardado: %p\n", MessageBoxWOriginal);
    }
};