#include <VirtualMemory.h>
#include <sys/mman.h>
#include <stdio.h>
void* AllocateVirtualMemory(size_t size)
{
    void* addr = mmap(nullptr, size, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    if(addr == MAP_FAILED)
    {
        perror("Error mmap:");
        return nullptr;
    }
    return addr;
}

void FreeVirtualMemory(void* mem, size_t size)
{
    if(munmap(mem, size) != 0)
    {
        perror("Error munmap:");
        return;
    }
}
