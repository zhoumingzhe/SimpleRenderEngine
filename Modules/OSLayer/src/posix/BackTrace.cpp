#include "BackTrace.h"
#include <execinfo.h>

size_t GetBackTrace(void** buffer, size_t size)
{
    return backtrace(buffer, size);
}