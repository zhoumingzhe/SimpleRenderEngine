#include <Allocator.h>
class StackFrame
{
    size_t frame;
public:
    bool Used()
    {
        return frame & 1;
    }

    void Used(bool bUsed)
    {
        frame = (frame & ~size_t(1)) | size_t(!!bUsed);
    }

    StackFrame* Ptr()
    {
        return (StackFrame*)(frame & ~size_t(1));
    }

    void Ptr(StackFrame * ptr);
};

class StackAllocator: public Allocator
{
public:
    StackAllocator(size_t total);
    StackAllocator(const StackAllocator& rhs) = delete;

    ~StackAllocator();

    void* Malloc(size_t size) override;
    void Free(void* mem) override;
    void Destroy() override;

    static StackAllocator* CreateStackAllocator(size_t totalSize);

private:
    size_t m_totalSize;
    StackFrame* m_pLast;
};


