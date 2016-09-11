#include <stddef.h>
class Allocator
{
public:
    virtual ~Allocator(){};
    virtual void* Malloc(size_t size) = 0;
    virtual void Free(void* mem) = 0;
    virtual void Destroy() = 0;
#if SRE_DBG
    //Returns true if no error(leak or corruption) is found
    //otherwise false
    virtual bool Check() = 0;
#endif
};

Allocator* CreateStackAllocator(size_t size);