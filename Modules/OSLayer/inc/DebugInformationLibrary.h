#include <stddef.h>

struct DebugInformationLibrary;

DebugInformationLibrary* InitializeDebugInformationLibrary();

void CleanupDebugInformation(DebugInformationLibrary* pDebugInfoLibrary);

bool TranslateDebugInfo(DebugInformationLibrary* pDebugInfoLibrary, void* addr, char* file, size_t fileLength, size_t* line, char* symbolName, size_t symbolLenth);

bool Demangle(DebugInformationLibrary* pDebugInfoLibrary, const char* symbolName, char* demangled, size_t demangledLength);
