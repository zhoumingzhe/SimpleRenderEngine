#include <Allocator.h>

struct StackFrame
{
    size_t prev;
    size_t next;
};

template <size_t alignment>
class StackAllocator: public Allocator
{
public:
    StackAllocator(size_t total);
    StackAllocator(const StackAllocator& rhs) = delete;
    void* malloc(size_t size) override;
    void free(void* mem) override;
    void destroy() override;
    static StackAllocator* CreateStackAllocator(size_t totalSize);

private:
    size_t m_totalSize;
    StackFrame head;
    typedef StackAllocator<alignment> this_type;
    static_assert((alignment&(alignment-1))==0, "Incorrect alignment");
};

