#include <gtest/gtest.h>
#include <Allocator.h>
TEST(TestStackAllocator, test)
{
    Allocator* pAllocator = CreateStackAllocator(4096*16);
    void * mem = pAllocator->Malloc(16);
    ASSERT_NE(nullptr, mem);
    memset(mem, 0, 16);
    pAllocator->Free(mem);
    pAllocator->Destroy();
}