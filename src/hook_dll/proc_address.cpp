#include "proc_address.h"

#include <string.h>

FARPROC GetProcAddressReplacement ( HMODULE module, LPCSTR api_name )
{
    PBYTE base = (PBYTE)module;

    PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)base;
    if ( dos_header->e_magic != IMAGE_DOS_SIGNATURE )
    {
        return NULL;
    }

    PIMAGE_NT_HEADERS nt_headers = (PIMAGE_NT_HEADERS)( base + dos_header->e_lfanew );
    if ( nt_headers->Signature != IMAGE_NT_SIGNATURE )
    {
        return NULL;
    }

    IMAGE_DATA_DIRECTORY export_data_dir = nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

    PIMAGE_EXPORT_DIRECTORY export_dir = (PIMAGE_EXPORT_DIRECTORY)( base + export_data_dir.VirtualAddress );

    PDWORD function_addresses = (PDWORD)( base + export_dir->AddressOfFunctions );

    PVOID function_address = NULL;

    // Lookup by ordinal
    if ( (ULONG_PTR)api_name <= 0xFFFF )
    {
        WORD ordinal = (WORD)( (ULONG_PTR)api_name & 0xFFFF );
        DWORD ordinal_base = export_dir->Base;

        if ( ordinal < ordinal_base || ordinal >= ordinal_base + export_dir->NumberOfFunctions )
        {
            return NULL;
        }

        function_address = (PVOID)( base + function_addresses[ordinal - ordinal_base] );
    }
    // Lookup by name
    else
    {
        PDWORD function_names = (PDWORD)( base + export_dir->AddressOfNames );
        PWORD function_ordinals = (PWORD)( base + export_dir->AddressOfNameOrdinals );

        for ( DWORD i = 0; i < export_dir->NumberOfNames; i++ )
        {
            CHAR* function_name = (CHAR*)( base + function_names[i] );
            if ( strcmp ( api_name, function_name ) == 0 )
            {
                function_address = (PVOID)( base + function_addresses[function_ordinals[i]] );
                break;
            }
        }
    }

    return (FARPROC)function_address;
}
