# Windows IAT Hooking

Intercept Win32 API calls by patching the Import Address Table

## How does it work?

This project demonstrates IAT (Import Address Table) hooking by intercepting calls to `MessageBoxW` from `USER32.dll`. The DLL replaces the function pointer in the target executable's IAT with a custom handler, redirecting all calls to our code.

The hook works by manually parsing the PE (Portable Executable) format of the target module: walking the DOS header, NT headers, and the import data directory to locate the `MessageBoxW` thunk. Once found, `VirtualProtect` temporarily lifts the page protection, the pointer is overwritten with the address of our hook function, and the original protection is restored. From that point on, every call to `MessageBoxW` lands in our code first, where parameters can be logged, modified, or the call blocked entirely.

The project also includes manual reimplementations of `GetModuleHandle` (via PEB walking) and `GetProcAddress` (via Export Directory parsing), built from scratch using only the Windows SDK — no Detours, no MinHook.

#Ex

MAKE SURE TO BUILD AND RUN THE EXE AND DLL FOR THE SAME ARCHITECTURE (x64 or x86). MIXING ARCHITECTURES WILL CAUSE `LoadLibrary` TO FAIL.

NOTE: THIS IS FOR EDUCATIONAL PURPOSES ONLY. I WON'T BE ADDING ANY ANTI-DETECTION OR PRODUCTION-READY CODE.

For more detailed technical analysis and study notes on PE format and IAT hooking, check out my personal study blog:

[Understanding the PE structure — Part 1](https://cnthigu.github.io/estrutura-pe-parte1/)

Note: This is my personal study blog with technical notes. If it helps with your learning, feel free to use!