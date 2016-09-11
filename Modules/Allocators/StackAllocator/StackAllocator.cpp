#include "StackAllocator.h"
#include <VirtualMemory.h>
#include <new>
#include <cstdint>
#include <cassert>

#if SRE_DBG
#include <stdio.h>
#endif

inline size_t align_down(size_t addr, size_t align)
{
    return addr&~(align-1);
}

inline size_t align_up(size_t addr, size_t align)
{
    return (addr + align -1) & ~(align - 1);
}

void StackFrame::Ptr(StackFrame * ptr)
{
    assert((size_t(ptr) & 1) == 0);
    frame = (size_t(ptr) & ~size_t(1)) | (frame&1);
}

StackAllocator::StackAllocator(size_t total)
:
m_totalSize(total),
m_pLast(BaseLocation())
{
    m_pLast->Ptr(nullptr);
    m_pLast->Used(false);
}

StackAllocator::~StackAllocator()
{

}

uint8_t* GetFrameMemoryLocation(StackFrame* pFrame)
{
    return (uint8_t*)(pFrame) + align_up(sizeof(StackFrame), 2);
}

StackFrame* GetFrameLocation(uint8_t* pMem)
{
    return (StackFrame*)((uint8_t*)pMem - align_up(sizeof(StackFrame), 2));
}

void* StackAllocator::Malloc(size_t size)
{
    size = align_up(size, 2);
    StackFrame* prev = m_pLast;
    uint8_t* mem = GetFrameMemoryLocation(m_pLast);
    uint8_t* tail = mem + size;

    if(tail - (uint8_t*)this > (ptrdiff_t)m_totalSize)
    {
        //TODO: Error report
        return nullptr;
    }

    prev->Used(true);

    m_pLast = (StackFrame*)tail;
    m_pLast->Ptr(prev);
    m_pLast->Used(false);
    return mem;
}

void StackAllocator::Free(void* mem)
{
    StackFrame* pFrame = GetFrameLocation((uint8_t*)mem);
    pFrame->Used(false);

    StackFrame* last = m_pLast;
    StackFrame* ptr = m_pLast->Ptr();
    while(ptr && !ptr->Used())
    {
        last = ptr;
        ptr = ptr->Ptr();
    }
    m_pLast = last;
}

void StackAllocator::Destroy()
{
    size_t size = m_totalSize;
    this->~StackAllocator();
    FreeVirtualMemory(this, size);
}

#if SRE_DBG
bool StackAllocator::Check()
{
    StackFrame* pBase = BaseLocation();
    if(m_pLast - pBase < 0)
    {
        //Buffer overrun and frame pointer corruption
        printf("Internal Structure Error\n");
        return false;
    }

    //nothing allocated
    if(m_pLast == pBase)
    {
        if(pBase->Used())
        {
            printf("Internal Structure Error\n");
            return false;
        }
        return true;
    }

    bool bAllFreed = true;
    for(StackFrame* pFrame = m_pLast->Ptr(); pFrame != nullptr; pFrame = pFrame->Ptr())
    {
        if(pFrame->Used())
        {
            bAllFreed = false;
            printf("Memory %p allocated but not freed\n", GetFrameMemoryLocation(pFrame));
        }
    }

    return bAllFreed;
}
#endif

StackFrame* StackAllocator::BaseLocation()
{
    return (StackFrame*)align_up(size_t(this + 1) , 2);
}

StackAllocator* StackAllocator::CreateStackAllocator(size_t totalSize)
{
    void* mem = AllocateVirtualMemory(totalSize);
    if(mem)
    {
        return new(mem) StackAllocator(totalSize);
    }
    else
    {
        //error reporting already included in AllocateVirtualMemory
        return nullptr;
    }
}

Allocator* CreateStackAllocator(size_t size)
{
    return StackAllocator::CreateStackAllocator(size);
}
