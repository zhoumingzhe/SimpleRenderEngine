#include <VirtualMemory.h>
#include <windows.h>
#include <stdio.h>
void* AllocateVirtualMemory(size_t size)
{
    void* addr = VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if(!addr)
    {
        printf("Error VirtualAlloc£º %lu", GetLastError());
    }
    return addr;
}

void FreeVirtualMemory(void* mem, size_t)
{
    if(!VirtualFree(mem, 0, MEM_RELEASE))
    {
        printf("Error VirtualFree£º %lu", GetLastError());
        return;
    }
}
