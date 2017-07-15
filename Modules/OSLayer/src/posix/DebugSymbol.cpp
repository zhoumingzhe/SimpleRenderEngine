#include <DebugSymbol.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <unistd.h>
#include <map>
#include <vector>
#include <bfd.h>
#include <elf.h>
#include <assert.h>
#include <cxxabi.h>

static void bfd_error_handler(const char *, ...)
{
}

bool InitializeSymbolLibrary()
{
    bfd_init();
    bfd_set_error_handler(bfd_error_handler);
    return true;
}

const char* GetModuleName(void* addr)
{
    Dl_info dlInfo;
    if(!dladdr(addr, &dlInfo))
    {
        return nullptr;
    }
    return dlInfo.dli_sname;
}

struct DebugInfo
{
    std::string module;
    bfd* pBfd;
    std::vector<bfd_symbol*> bfdSymbol;
    DebugInfo(const char* name)
        :module(name)
    {
    }
    ~DebugInfo()
    {
        bfd_close(pBfd);
    }
};



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

    std::vector<bfd_symbol*> buffer(storage/sizeof(bfd_symbol*));
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

    DebugInfo* pDebugInfo = new DebugInfo(module);
    pDebugInfo->pBfd = pbfd;
    pDebugInfo->bfdSymbol.swap(buffer);
    return pDebugInfo;
}

void CloseDebugInfo(DebugInfo* dbgInfo)
{
    delete dbgInfo;
}

bool TranslateDebugInfo(DebugInfo* pDebugInfo, void* addr, const char** ppFileName, size_t* pLine, const char** ppSymbolName)
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

        unsigned int line = 0;
        if (1 == bfd_find_nearest_line(pBfd, pSect, pDebugInfo->bfdSymbol.data(), vma - pSect->vma,  
                                               ppFileName, ppSymbolName, &line)) 
        {
            *pLine = line;
            return true;
        }
    }
    return false;
}
char* Demangle(DebugInfo* pDebugInfo, const char* symbolName)
{
    int status = 0;
    return __cxxabiv1::__cxa_demangle(symbolName, 0, 0, &status);
}