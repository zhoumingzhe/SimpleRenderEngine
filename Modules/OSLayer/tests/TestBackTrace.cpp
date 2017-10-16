#include <BackTrace.h>
#include <DebugInformationLibrary.h>
#include <cassert>
#include <stdio.h>
#include <malloc.h>
template <typename T, size_t N>
constexpr size_t countof(T const (&)[N]) noexcept
{
    return N;
}
int main(int, char**)
{
    void* buffer[64] = {};
    size_t size = GetBackTrace(buffer, sizeof(buffer)/sizeof(buffer[0]));
    assert(size>0);
    DebugInformationLibrary* pDebugInfo = InitializeDebugInformationLibrary();
    char symbolName[1024];
    char file[1024] = "";
    size_t line;
    for(size_t i = 0; i < size; ++i)
    {
        if (TranslateDebugInfo(pDebugInfo, buffer[i], file, countof(file) - 1, &line, symbolName, countof(symbolName) - 1))
        {
            char demangled[1024];
            bool success = Demangle(pDebugInfo, symbolName, demangled, countof(demangled) - 1);
            printf("[%s] %s, %s:%zu\n", success?"C++":"C", success?demangled:symbolName, file, line);
        }
    }
    CleanupDebugInformation(pDebugInfo);
    return 0;
}
