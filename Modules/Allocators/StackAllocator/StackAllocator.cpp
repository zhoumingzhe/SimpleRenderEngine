#include "StackAllocator.h"
#include <VirtualMemory.h>
#include <new>
#include <assert.h>
struct StackFrame
{
    
};

template <size_t alignment>
StackAllocator<alignment>::StackAllocator(size_t total)
:
m_totalSize(total)
{
}

template <size_t alignment>
void* StackAllocator<alignment>::malloc(size_t size)
{
    return nullptr;
}

template <size_t alignment>
void StackAllocator<alignment>::free(void* mem)
{

}

template <size_t alignment>
void StackAllocator<alignment>::destroy()
{
    FreeVirtualMemory(this, m_totalSize);
}

//Why allocate the memory outside of the constructor:
//If no memory, null object will be returned to the outside
template <size_t alignment>
StackAllocator<alignment>* StackAllocator<alignment>::CreateStackAllocator(size_t size)
{
    void* pMem = AllocateVirtualMemory(size);
    if(pMem)
    {
        StackAllocator* pAllocator = new (pMem) StackAllocator<alignment>(size);
        return pAllocator;
    }
    else
    {
        //TODO: error handling
        return nullptr;
    }
}

Allocator* CreateStackAllocator(size_t size, size_t alignment)
{
    //align to 4K is enough 
    assert((0 < alignment) && (alignment <= 12) && (alignment & (alignment - 1)) == 0);
#define CASE(i) case 1<<(i-1): return StackAllocator< 1<<(i-1) >::CreateStackAllocator(size)
    switch(alignment)
    {
        //nasty code, need to find a better way
        CASE(1);
        CASE(2);
        CASE(3);
        CASE(4);
        CASE(5);
        CASE(6);
        CASE(7);
        CASE(8);
        CASE(9);
        CASE(10);
        CASE(12);
        default:
            assert(0);
    }
    return nullptr;
}