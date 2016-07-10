#include <Allocator.h>

class StackAllocator: public Allocator
{
public:
    StackAllocator(void* base, size_t total);
    StackAllocator(const StackAllocator& rhs) = delete;
    void* malloc(size_t size) override;
    void free(void* mem) override;
    void destroy() override;

    static StackAllocator* CreateStackAllocator(size_t totalSize);
private:
    void* m_base;
    size_t m_totalSize;
};
