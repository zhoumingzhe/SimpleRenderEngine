#include <stddef.h>
class Allocator
{
public:
    virtual ~Allocator(){};
    virtual void* Malloc(size_t size) = 0;
    virtual void Free(void* mem) = 0;
    virtual void Destroy() = 0;
};

Allocator* CreateStackAllocator(size_t size);