#include <stddef.h>

struct DebugInformationLibrary;

DebugInformationLibrary* InitializeDebugInformationLibrary();

void CleanupDebugInformation(DebugInformationLibrary* pDebugInfoLibrary);

bool TranslateDebugInfo(DebugInformationLibrary* pDebugInfoLibrary, void* addr, const char** file, size_t* line, const char** symbolName);

char* Demangle(DebugInformationLibrary* pDebugInfoLibrary, const char* symbolName);
