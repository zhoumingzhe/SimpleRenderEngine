#include <gtest/gtest.h>
#include <Allocator.h>
TEST(TestStackAllocator, test)
{
    Allocator* pAllocator = CreateStackAllocator(4096*16);
    void* mem1 = pAllocator->Malloc(16);
    ASSERT_NE(nullptr, mem1);
    memset(mem1, 0, 16);

    void* mem2 = pAllocator->Malloc(32);
    ASSERT_NE(nullptr, mem2);
    memset(mem2, 0, 16);

    void* mem3 = pAllocator->Malloc(48);
    ASSERT_NE(nullptr, mem3);
    memset(mem3, 0, 16);
    pAllocator->Free(mem1);
    pAllocator->Free(mem2);
#if SRE_DBG
    ASSERT_FALSE(pAllocator->Check());
#endif
    pAllocator->Free(mem3);

#if SRE_DBG
    ASSERT_NE(false, pAllocator->Check());
#endif
    pAllocator->Destroy();
    
}