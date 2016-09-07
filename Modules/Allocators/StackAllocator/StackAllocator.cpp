#include "StackAllocator.h"
#include <VirtualMemory.h>
#include <new>
#include <cstdint>
#include <cassert>

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
m_pLast((StackFrame*)align_up(size_t(this + 1) , 2))
{
    m_pLast->Ptr(nullptr);
    m_pLast->Used(false);
}

StackAllocator::~StackAllocator()
{

}

void* StackAllocator::Malloc(size_t size)
{
    size = align_up(size, 2);
    StackFrame* prev = m_pLast;
    uint8_t* mem = (uint8_t*)(m_pLast) + align_up(sizeof(StackFrame), 2);
    uint8_t* tail = mem + size;

    if(tail - (uint8_t*)this > m_totalSize)
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
    StackFrame* pFrame = (StackFrame*)((uint8_t*)mem - align_up(sizeof(StackFrame), 2));
    pFrame->Used(false);

    StackFrame* last = m_pLast;
    StackFrame* ptr = m_pLast->Ptr();
    while(ptr && !ptr->Used())
    {
        last = ptr;
        ptr = ptr->Ptr();
    }
    m_pLast = ptr;
}

void StackAllocator::Destroy()
{
    size_t size = m_totalSize;
    StackAllocator::~StackAllocator();
    FreeVirtualMemory(this, size);
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
        //TODO: error reporting
        return nullptr;
    }
}

Allocator* CreateStackAllocator(size_t size)
{
    return StackAllocator::CreateStackAllocator(size);
}
