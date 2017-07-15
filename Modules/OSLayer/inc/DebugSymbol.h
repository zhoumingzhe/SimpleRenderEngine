#include <cstddef>
bool InitializeSymbolLibrary();
void CleanupSymbolLibrary();

const char* GetModuleName(void* addr);

struct DebugInfo;
DebugInfo* LoadDebugInfo(const char* module);
void CloseDebugInfo(DebugInfo* dbgInfo);

bool TranslateDebugInfo(DebugInfo* pDebugInfo, void* addr, const char** file, size_t* line, const char** symbolName);
char* Demangle(DebugInfo* pDebugInfo, const char* symbolName);