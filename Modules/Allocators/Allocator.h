#include <stddef.h>
class Allocator
{
public:
    virtual ~Allocator(){};
    virtual void* malloc(size_t size) = 0;
    virtual void free(void* mem) = 0;
    virtual void destroy() = 0;
};

Allocator* CreateStackAllocator(size_t size);