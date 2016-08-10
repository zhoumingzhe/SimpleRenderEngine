#include <gtest/gtest.h>
#include <Allocator.h>
TEST(TestStackAllocator, test)
{
    for(size_t alignment = 0; alignment<12; ++alignment)
    {
        Allocator* pAllocator = CreateStackAllocator(4096*16, 1<<alignment);
        void * mem = pAllocator->malloc(16);
        size_t addr = (size_t)mem;
        ASSERT_NE(nullptr, mem);
        ASSERT_EQ(size_t(0), addr&((1<<alignment)-1));
        memset(mem, 0, 16);
        pAllocator->free(mem);
        pAllocator->destroy();
    }
}