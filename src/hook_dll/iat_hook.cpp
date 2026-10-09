#include "iat_hook.h"

#include "util.h"

#include <string.h>

static PVOID PatchIATEntry ( PBYTE target, PCSTR api_name, PIMAGE_IMPORT_DESCRIPTOR module_entry, LPCVOID replacement )
{
    PULONG_PTR original_thunk = (PULONG_PTR)( target + module_entry->OriginalFirstThunk );
    PULONG_PTR thunk = (PULONG_PTR)( target + module_entry->FirstThunk );

    while ( *original_thunk != NULL )
    {
        if ( IMAGE_SNAP_BY_ORDINAL ( *original_thunk ) )
        {
            original_thunk++;
            thunk++;
            continue;
        }

        PIMAGE_IMPORT_BY_NAME import_by_name = (PIMAGE_IMPORT_BY_NAME)( target + *original_thunk );

        if ( strcmp ( import_by_name->Name, api_name ) == 0 )
        {
            PVOID original = (PVOID)( *thunk );

            DWORD protect = 0;
            VirtualProtect ( thunk, sizeof ( ULONG_PTR ), PAGE_READWRITE, &protect );
            *thunk = (ULONG_PTR)replacement;
            VirtualProtect ( thunk, sizeof ( ULONG_PTR ), protect, &protect );

            return original;
        }

        original_thunk++;
        thunk++;
    }

    return NULL;
}

PVOID HookIAT ( PBYTE target, LPCSTR module_name, LPCSTR api_name, LPCVOID replacement )
{
    PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)target;
    if ( dos_header->e_magic != IMAGE_DOS_SIGNATURE )
    {
        return NULL;
    }

    PIMAGE_NT_HEADERS nt_headers = (PIMAGE_NT_HEADERS)( target + dos_header->e_lfanew );
    if ( nt_headers->Signature != IMAGE_NT_SIGNATURE )
    {
        return NULL;
    }

    IMAGE_DATA_DIRECTORY import_data_dir = nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

    PIMAGE_IMPORT_DESCRIPTOR import_table = (PIMAGE_IMPORT_DESCRIPTOR)( target + import_data_dir.VirtualAddress );

    SIZE_T import_count = import_data_dir.Size / sizeof ( IMAGE_IMPORT_DESCRIPTOR );

    for ( SIZE_T i = 0; i < import_count; i++ )
    {
        if ( import_table[i].Name == 0 )
        {
            break;
        }

        char* imported_module_name = (char*)( target + import_table[i].Name );

        if ( IsEqualCStr ( module_name, imported_module_name ) )
        {
            PVOID original = PatchIATEntry ( target, api_name, &import_table[i], replacement );

            if ( original != NULL )
            {
                return original;
            }
        }
    }

    return NULL;
}
