#include <stddef.h>
extern "C"
{
    void* AllocateVirtualMemory(size_t size);
    void FreeVirtualMemory(void* mem, size_t size);
}