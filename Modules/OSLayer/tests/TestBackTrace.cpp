#include <BackTrace.h>
#include <cassert>
int main()
{
    void* buffer[64] = {};
    size_t size = GetBackTrace(buffer, sizeof(buffer)/sizeof(buffer[0]));
    assert(size>0);
    
    return 0;
}