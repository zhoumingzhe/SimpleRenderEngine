#include <VirtualMemory.h>

int main()
{
    void* pMem = AllocateVirtualMemory(4*1024);
    FreeVirtualMemory(pMem, 4*1024);
    return 0;
}