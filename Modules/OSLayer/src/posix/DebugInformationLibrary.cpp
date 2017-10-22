#include "DebugInformationLibrary.h"
#include <bfd.h>
#include <vector>
#include <map>
#include <dlfcn.h>
#include <assert.h>
#include <cxxabi.h>
#include <stdarg.h>
#include <string.h>

struct DebugInfo
{
    bfd* pBfd;
    std::vector<bfd_symbol*> bfdSymbol;
    ~DebugInfo()
    {
        bfd_close(pBfd);
    }
};

struct DebugInformationLibrary
{
    std::map<const char*, DebugInfo*> debugInfo;
};

extern DebugInformationLibrary* (*fnInitializeDebugInformationLibrary)();

void bfd_error_handler(const char *, va_list va)
{
}

void bfd_error_handler(const char * c, ...)
{
    va_list args;
    va_start(args, c);
    bfd_error_handler(c, args);
    va_end(args);
}

template<bool globalInit>
DebugInformationLibrary* CreateDebugInformationLibrary()
{
    if(globalInit)
    {
        bfd_init();
        bfd_set_error_handler(bfd_error_handler);
        fnInitializeDebugInformationLibrary = CreateDebugInformationLibrary<false>;
    }
    return new DebugInformationLibrary;
}

DebugInformationLibrary* InitializeDebugInformationLibrary()
{
    return fnInitializeDebugInformationLibrary();
}

DebugInformationLibrary* (*fnInitializeDebugInformationLibrary)() = CreateDebugInformationLibrary<true>;

void CleanupDebugInformation(DebugInformationLibrary* pLibrary)
{
    delete pLibrary;
}

static const char* GetModuleName(void* addr)
{
    Dl_info dlInfo;
    if(!dladdr(addr, &dlInfo))
    {
        return nullptr;
    }
    return dlInfo.dli_fname;
}

DebugInfo* LoadDebugInfo(const char* module)
{
    bfd* pbfd = bfd_openr(module, nullptr);
    if(!pbfd)
    {
        return nullptr;
    }

    if(bfd_check_format(pbfd, bfd_archive))
        return nullptr;

    if(!bfd_check_format(pbfd, bfd_object))
        return nullptr;

    if(0 == (bfd_get_file_flags(pbfd) & HAS_SYMS))
        return nullptr;

    long storage = bfd_get_symtab_upper_bound(pbfd);

    if(storage < 0)
    {
        return nullptr;
    }

    DebugInfo* pDebugInfo = new DebugInfo{pbfd, std::vector<bfd_symbol*>{storage/sizeof(bfd_symbol*)}};
    auto& buffer = pDebugInfo->bfdSymbol;
    bfd_symbol** ppBfdSymbol = buffer.data();
    long symbolCount = bfd_canonicalize_symtab(pbfd, ppBfdSymbol);
    assert(symbolCount*(long)sizeof(bfd_symbol*)<=storage);
    if(symbolCount == 0)
    {
        storage = bfd_get_dynamic_symtab_upper_bound(pbfd);

        if(storage < 0)
            return nullptr;
        buffer.resize(storage/sizeof(bfd_symbol*));
        ppBfdSymbol = buffer.data();
        symbolCount = bfd_canonicalize_dynamic_symtab(pbfd, ppBfdSymbol);
        assert(symbolCount*(long)sizeof(bfd_symbol*)<=storage);
    }
    buffer.resize(symbolCount);

    return pDebugInfo;
}

bool TranslateDebugInfo(DebugInfo* pDebugInfo, void* addr, char* file, size_t fileLength, size_t* pLine, char* symbolName, size_t symbolLength)
{
    bfd_vma vma = (bfd_vma)addr;
    bfd* pBfd = pDebugInfo->pBfd;
    // We never hit the code below on x86_64 linux.
    // Comment out for now.
    //if (bfd_get_flavour (pBfd) == bfd_target_elf_flavour)  
    //{
        //if(bfd_get_sign_extend_vma(pBfd))
        //{
            //bfd_vma sign = (bfd_vma) 1 << (bfd_get_arch_size(pBfd) - 1);
            //vma &= (sign << 1) - 1;
            //vma = (vma ^ sign) - sign;
        //}
    //}

    asection* pSect = nullptr;
    for (pSect = pBfd->sections; pSect != NULL; pSect = pSect->next)  
    {
        if ((pSect->flags & SEC_ALLOC) == 0)  
        {
            continue;
        }

        ///section start position  
        if (vma < pSect->vma)  
        {
            continue;
        }

        ///section end position  
        if (vma >= pSect->vma + pSect->size)  
        {
            continue;
        }

        const char* pFileName = nullptr;
        const char* pSymbolName = nullptr;
        unsigned int line = 0;
        if (1 == bfd_find_nearest_line(pBfd, pSect, pDebugInfo->bfdSymbol.data(), vma - pSect->vma,  
                                               &pFileName, &pSymbolName, &line)) 
        {
            strncpy(file, pFileName, fileLength);
            strncpy(symbolName, pSymbolName, symbolLength);
            *pLine = line;
            return true;
        }
    }
    return false;
}

bool TranslateDebugInfo(DebugInformationLibrary* pDebugInfoLibrary, void* addr, char* file, size_t fileLength, size_t* line, char* symbolName, size_t symbolLength)
{
    const char* moduleName = GetModuleName(addr);
    if(moduleName == nullptr)
        return false;

    DebugInfo* pDebugInfo = nullptr;
    auto iter = pDebugInfoLibrary->debugInfo.find(moduleName);
    if(iter == pDebugInfoLibrary->debugInfo.end())
    {
        pDebugInfo = LoadDebugInfo(moduleName);
        if(!pDebugInfo)
            return false;
        pDebugInfoLibrary->debugInfo.insert(std::make_pair(moduleName, pDebugInfo));
    }
    else
        pDebugInfo = iter->second;
    
    return TranslateDebugInfo(pDebugInfo, addr, file, fileLength, line, symbolName, symbolLength);
}

bool Demangle(DebugInformationLibrary* pDebugInfoLibrary, const char* symbolName, char* demangled, size_t demangledLength)
{
    int status = 0;
    const char* result = __cxxabiv1::__cxa_demangle(symbolName, 0, 0, &status);
    if(result)
    {
        strncpy(demangled, result, demangledLength);
        return true;
    }
    return false;
}
