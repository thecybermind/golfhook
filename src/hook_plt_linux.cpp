/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#if defined(__linux__)

#include <link.h>
#include <dlfcn.h>
#include <stdio.h>
#include <elf.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/limits.h>
#include "hook_iat.h"


#define ALIGN_ADDR(addr) ((void*)((uintptr_t)addr & ~(page_size - 1)))
void* install_IAT_hook(void* target_module, const char*, const char* function_name, void* function_hook) {
    size_t page_size = sysconf(_SC_PAGESIZE);   // need to store size of page for mprotect

    size_t funcnamelen = strlen(function_name);	// store length of function name for comparison (check first X bytes, see if next byte is '\0' or '@')

    struct link_map* lmap = nullptr;		    // from dlinfo man page: pointer to dynamic section of the shared object
    const char* plt_addr_base = nullptr;	    // from dlinfo man page: difference between an address in the ELF file and the address in memory

    const Elf64_Sym* dynsym = nullptr;		    // DT_SYMTAB: address of dynamic symbol table
    const char* dynstr = nullptr;			    // DT_STRTAB: dynamic string table
    size_t dynstr_size = 0;				        // DT_STRSZ: total size in bytes of the dynamic string table (DT_STRTAB)
    const Elf64_Rel* rela_plt = nullptr;        // DT_JMPREL: relocation entries
    size_t rela_plt_cnt = 0;			        // DT_PLTRELSZ: total size in bytes of the relocation entries (DT_PLTRELSZ)

    const char* symbol_name;			        // current name when looping through symbols
    void** symbol_addr;					        // current address when looping through symbols

    void* ret = nullptr;                        // store and return old function address

    // get dynamic section of target module
    if (dlinfo(target_module, RTLD_DI_LINKMAP, &lmap) != 0) {
        printf("dlinfo failed!\n");
        return nullptr;
    }

    plt_addr_base = (char*)lmap->l_addr;

    // go through all the sections and save various ones
    for (const Elf64_Dyn* dyn = lmap->l_ld; dyn->d_tag != DT_NULL; dyn++) {
        // .dynsym section
        if (dyn->d_tag == DT_SYMTAB) {
            dynsym = (const Elf64_Sym*)(dyn->d_un.d_ptr);
        }
        // check sizeof Elf64_Sym
        else if (dyn->d_tag == DT_SYMENT) {
            if (dyn->d_un.d_val != sizeof(Elf64_Sym)) {
                printf("DT_SYMENT wrong size\n");
                return nullptr;
            }
        }
        // .dynstr section
        else if (dyn->d_tag == DT_STRTAB) {
            dynstr = (const char*)dyn->d_un.d_ptr;
        }
        // .dynstr size
        else if (dyn->d_tag == DT_STRSZ) {
            dynstr_size = dyn->d_un.d_val;
        }
        // .rela.plt or .rel.plt section
        else if (dyn->d_tag == DT_JMPREL) {
            rela_plt = (const Elf64_Rel*)(dyn->d_un.d_ptr);
        }
        // .rela.plt or .rel.plt section size
        else if (dyn->d_tag == DT_PLTRELSZ) {
            rela_plt_cnt = dyn->d_un.d_val / sizeof(Elf64_Rel);
        }
    }
    // make sure we got everything we need
    if (!dynsym || !dynstr || !dynstr_size || !rela_plt || !rela_plt_cnt)
        return nullptr;

    // loop through all relocations
    for (const Elf64_Rel* plt = rela_plt; plt != rela_plt + rela_plt_cnt; plt++) {
        // low byte of r_info is the type, high 3 bytes of r_info is the symbol index
        if (ELF64_R_TYPE(plt->r_info) == R_X86_64_JUMP_SLOT) {
            size_t idx = ELF64_R_SYM(plt->r_info);
            // get string index for symbol
            idx = dynsym[idx].st_name;
            // string index out of bounds, skip
            if (idx + 1 > dynstr_size)
                continue;
            symbol_name = dynstr + idx;
            symbol_addr = (void**)(plt_addr_base + plt->r_offset);
            // if this symbol matches the given function name
            if (!strncmp(symbol_name, function_name, funcnamelen) && (symbol_name[funcnamelen] == '\0' || symbol_name[funcnamelen] == '@')) {
                if (mprotect(ALIGN_ADDR(symbol_addr), page_size, PROT_READ | PROT_WRITE) != 0)
                    return nullptr;
                ret = *symbol_addr;
                *symbol_addr = function_hook;
                // mprotect(ALIGN_ADDR(symbol_addr), page_size, PROT_READ | PROT_EXEC);
                return ret;
            }
        }
    }

    return nullptr;
}

#endif // __linux__
