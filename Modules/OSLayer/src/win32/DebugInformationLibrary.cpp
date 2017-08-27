#include "DebugInformation.h"
#include <windows.h>
#include <DbgHelp.h>
bool InitializeDebugInformation()
{
    SymSetOptions(SYMOPT_LOAD_LINES);
    return !!SymInitialize(GetCurrentProcess(), nullptr, TRUE);
}

bool CleanupDebugInformation()
{
    return !!SymCleanup(GetCurrentProcess());
}
