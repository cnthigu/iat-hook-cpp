#include "module_handle.h"

#include <winternl.h>

static BOOL IsModuleNameEqual ( LPCSTR ansi_name, PUNICODE_STRING wide_name )
{
    if ( ansi_name == NULL || wide_name == NULL || wide_name->Buffer == NULL )
    {
        return FALSE;
    }

    USHORT wide_length = wide_name->Length / sizeof ( WCHAR );

    USHORT i = 0;
    for ( ; i < wide_length && ansi_name[i] != '\0'; i++ )
    {
        WCHAR wide_char = wide_name->Buffer[i];
        CHAR ansi_char = ansi_name[i];

        if ( wide_char >= L'A' && wide_char <= L'Z' )
        {
            wide_char += 32;
        }

        if ( ansi_char >= 'A' && ansi_char <= 'Z' )
        {
            ansi_char += 32;
        }

        if ( (WCHAR)ansi_char != wide_char )
        {
            return FALSE;
        }
    }

    return ( i == wide_length && ansi_name[i] == '\0' );
}

HMODULE GetModuleHandleReplacement ( LPCSTR module_name )
{
#ifdef _WIN64
    PPEB peb = (PPEB)__readgsqword ( 0x60 );
#else
    PPEB peb = (PPEB)__readfsdword ( 0x30 );
#endif

    PLIST_ENTRY head = &peb->Ldr->InMemoryOrderModuleList;
    PLIST_ENTRY current = head->Flink;

    while ( current != head )
    {
        PLDR_DATA_TABLE_ENTRY entry = CONTAINING_RECORD ( current, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks );

        if ( module_name == NULL )
        {
            return (HMODULE)entry->DllBase;
        }

        // Reserved4 holds the BaseDllName UNICODE_STRING in the public LDR_DATA_TABLE_ENTRY
        if ( IsModuleNameEqual ( module_name, (PUNICODE_STRING)&entry->Reserved4 ) )
        {
            return (HMODULE)entry->DllBase;
        }

        current = current->Flink;
    }

    return NULL;
}
