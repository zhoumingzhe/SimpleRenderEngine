#include "DebugInformationLibrary.h"
#include <windows.h>
#include <DbgHelp.h>

struct DebugInformationLibrary
{
};

extern DebugInformationLibrary* (*fnInitializeDebugInformationLibrary)();

template<bool globalInit>
DebugInformationLibrary* CreateDebugInformationLibrary()
{
    if (globalInit)
    {
        SymSetOptions(SYMOPT_LOAD_LINES);
        SymInitialize(GetCurrentProcess(), nullptr, TRUE);
        fnInitializeDebugInformationLibrary = CreateDebugInformationLibrary<false>;
    }
    return new DebugInformationLibrary;
}

DebugInformationLibrary* InitializeDebugInformationLibrary()
{
    return fnInitializeDebugInformationLibrary();
}

DebugInformationLibrary* (*fnInitializeDebugInformationLibrary)() = CreateDebugInformationLibrary<true>;

void CleanupDebugInformation(DebugInformationLibrary* pLibrary)
{
    delete pLibrary;
}

bool TranslateDebugInfo(DebugInformationLibrary*, void* addr, char* file, size_t fileLength, size_t* line, char* symbolName, size_t symbolLength)
{
    DWORD64 displacement64 = 0;
    char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
    PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;
    pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    pSymbol->MaxNameLen = MAX_SYM_NAME;
    if (!SymFromAddr(GetCurrentProcess(), (DWORD64)addr, &displacement64, pSymbol))
    {
        return false;
    }

    IMAGEHLP_LINE64 linedata = { sizeof(linedata) };
    DWORD displacementDW = 0;
    if (!SymGetLineFromAddr64(GetCurrentProcess(), (DWORD64)addr, &displacementDW, &linedata))
    {
        return false;
    }

    strncpy(file, linedata.FileName, fileLength);
    *line = linedata.LineNumber;
    strncpy(symbolName, pSymbol->Name, symbolLength);
    return true;
}

bool Demangle(DebugInformationLibrary*, const char* symbolName, char* demangled, size_t demangledLength)
{
    strncpy(demangled, symbolName, demangledLength);
    return true;
}