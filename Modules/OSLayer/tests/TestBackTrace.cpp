#include <BackTrace.h>
#include <DebugSymbol.h>
#include <cassert>
#include <stdio.h>
#include <malloc.h>

int main(int argc, char** argv)
{
    void* buffer[64] = {};
    size_t size = GetBackTrace(buffer, sizeof(buffer)/sizeof(buffer[0]));
    assert(size>0);
    DebugInfo* pDebugInfo = LoadDebugInfo(argv[0]);
    const char* symbolName;
    const char* file;
    size_t line;
    for(size_t i = 0; i < size; ++i)
    {
        TranslateDebugInfo(pDebugInfo, buffer[i], &file, &line, &symbolName);
        char* demangled = Demangle(pDebugInfo, symbolName);
        printf("[%s] %s, %s:%zu\n", demangled?"C++":"C", demangled?demangled:symbolName, file, line);
        free(demangled);
    }
    CloseDebugInfo(pDebugInfo);
    return 0;
}