#include <cstddef>
#ifdef _WIN32
size_t GetBackTrace(void** buffer, size_t size);
#else
#include <execinfo.h>
//Avoid 1 more back trace generated in function call
#define GetBackTrace(buffer, size) backtrace(buffer, size)
#endif
