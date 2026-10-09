#include "util.h"

#include <stdio.h>

void EnableDebugConsole ()
{
    if ( AllocConsole () )
    {
        FILE* console_out;
        FILE* console_err;
        FILE* console_in;

        freopen_s ( &console_out, "CONOUT$", "w", stdout );
        freopen_s ( &console_err, "CONOUT$", "w", stderr );
        freopen_s ( &console_in, "CONIN$", "r", stdin );
    }
}

int IsEqualCStr ( const char* a, const char* b )
{
    if ( !a || !b )
    {
        return 0;
    }

    return _stricmp ( a, b ) == 0;
}
