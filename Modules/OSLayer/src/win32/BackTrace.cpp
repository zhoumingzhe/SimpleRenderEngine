#include "BackTrace.h"
#include <Windows.h>
size_t GetBackTrace(void** buffer, size_t size)
{
    return CaptureStackBackTrace(1, (DWORD)size, buffer, nullptr);
}