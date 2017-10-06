#include <DebugInformationLibrary.h>
int main(int, char**)
{
    DebugInformationLibrary* pDebugInfoLib = InitializeDebugInformationLibrary();

    CleanupDebugInformation(pDebugInfoLib);
    return 0;
}
