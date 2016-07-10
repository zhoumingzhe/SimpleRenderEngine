#include "StackAllocator.h"
#include <VirtualMemory.h>
#include <new>
StackAllocator::StackAllocator(void* base, size_t total)
:
m_base(base),
m_totalSize(total)
{
}

void* StackAllocator::malloc(size_t size)
{
    return nullptr;
}

void StackAllocator::free(void* mem)
{

}

void StackAllocator::destroy()
{
    FreeVirtualMemory(this, m_totalSize);
}

//Why allocate the memory outside of the constructor:
//If no memory, null object will be returned to the outside
StackAllocator* StackAllocator::CreateStackAllocator(size_t size)
{
    void* pMem = AllocateVirtualMemory(size);
    if(pMem)
    {
        StackAllocator* pAllocator = new (pMem) StackAllocator(pMem, size);
        return pAllocator;
    }
    else
    {
        //TODO: error handling
        return nullptr;
    }
}

Allocator* CreateStackAllocator(size_t size)
{
    return StackAllocator::CreateStackAllocator(size);
}