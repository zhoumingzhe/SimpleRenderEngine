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

bool TranslateDebugInfo(DebugInformationLibrary*, void* addr, const char** file, size_t* line, const char** symbolName)
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

    *file = linedata.FileName;
    *line = linedata.LineNumber;
    *symbolName = pSymbol->Name;

    return true;
}

char* Demangle(DebugInformationLibrary*, const char* symbolName)
{
    size_t len = strlen(symbolName) + 1;
    char* ret = (char*)malloc(len);
    strcpy(ret, symbolName);
    return ret;
}