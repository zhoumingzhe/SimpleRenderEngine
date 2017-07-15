#include <DebugSymbol.h>
int main(int argc, char** argv)
{
    DebugInfo* pDebugInfo = LoadDebugInfo("/lib/x86_64-linux-gnu/libc-2.23.so");
    CloseDebugInfo(pDebugInfo);
    return 0;
}