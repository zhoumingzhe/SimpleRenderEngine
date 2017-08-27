#include <DebugInformationLibrary.h>
int main(int argc, char** argv)
{
    DebugInformationLibrary* pDebugInfoLib = InitializeDebugInformationLibrary();

    CleanupDebugInformation(pDebugInfoLib);
    return 0;
}
