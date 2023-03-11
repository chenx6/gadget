#define _GNU_SOURCE
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <dlfcn.h>
#include <link.h>
#include <sys/mman.h>
#include <unistd.h>

#include "plt_hook.h"

static size_t page_size = 0;
#define ALIGN_ADDR(addr) ((void *)((size_t)(addr) & ~(page_size - 1)))
// Copied from dlopen's manual
#define DLOPEN_ERROR_HINT                                                      \
  "file could not be found, was not readable, had the wrong format, or "       \
  "caused errors during loading"

static void plog(const char *fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
  vfprintf(stderr, fmt, arg);
  fputc('\n', stderr);
  va_end(arg);
}

/// @brief Initlize plthook_info
/// @param info empty plthook_info variable
/// @param name library name, NULL means program itself
plthook_status plt_hook_init(plthook_info *info, char *name) {
  page_size = sysconf(_SC_PAGESIZE);
  if (info == NULL) {
    return PLTHOOK_ARGUMENT_ERROR;
  }
  plthook_status status = PLTHOOK_SUCCESS;
  // Get link_map
  void *handle = dlopen(name, RTLD_LAZY | RTLD_NOLOAD);
  if (handle == NULL) {
    plog("dlopen error, maybe " DLOPEN_ERROR_HINT);
    status = PLTHOOK_DLOPEN_ERROR;
    goto exit;
  }
  struct link_map *map = NULL;
  int result = dlinfo(handle, RTLD_DI_LINKMAP, &map);
  if (result != 0) {
    plog("dlinfo error");
    status = PLTHOOK_DLINFO_ERROR;
    goto exit;
  }
  // Get information from link_map's address and dyn table
  info->base_addr = (uint8_t *)map->l_addr;
  size_t rela_size = 0;
  for (Elf64_Dyn *curr = map->l_ld; curr->d_tag != DT_NULL; curr++) {
    switch (curr->d_tag) {
    // Address of string table
    case DT_STRTAB:
      info->str_table = curr->d_un.d_ptr;
      break;
    case DT_STRSZ:
      info->str_sz = curr->d_un.d_val;
      break;
    // Address of relocation entries associated solely with the PLT
    case DT_JMPREL:
      info->rel = (Elf64_Rela *)curr->d_un.d_ptr;
      break;
    case DT_PLTRELSZ:
      rela_size = curr->d_un.d_val;
      break;
    // Address of symbol table
    case DT_SYMTAB:
      info->dynsym = (Elf64_Sym *)curr->d_un.d_ptr;
      break;
    }
  }
  info->rel_cnt = rela_size / sizeof(Elf64_Rela);
  if (info->rel == NULL || info->rel_cnt == 0) {
    plog("Find .rela(relocation table) error");
    status = PLTHOOK_REL_ERROR;
    goto exit;
  }
  if (info->str_table == NULL || info->str_sz == 0) {
    plog("Find .strtab error");
    status = PLTHOOK_STR_ERROR;
    goto exit;
  }
  if (info->dynsym == NULL) {
    plog("FInd .dynsym error");
    status = PLTHOOK_SYMTAB_ERROR;
    goto exit;
  }
exit:
  if (handle != NULL) {
    dlclose(handle);
  }
  return status;
}

/// @brief Replace function with new function
plthook_status plt_hook_replace(plthook_info *info, char *name,
                                void *func_address) {
  if (info == NULL || name == NULL || func_address == NULL) {
    return PLTHOOK_ARGUMENT_ERROR;
  }
  // Get symbol index from relocation table, then get function name from symbol
  Elf64_Rela *rela = info->rel;
  for (size_t i = 0; i < info->rel_cnt; i++) {
    Elf64_Rela *curr = rela + i;
    size_t sym_idx = ELF64_R_SYM(curr->r_info);
    size_t str_idx = info->dynsym[sym_idx].st_name;
    if (strcmp(name, info->str_table + str_idx) != 0) {
      continue;
    }
    size_t *origin_func = (size_t *)(info->base_addr + curr->r_offset);
    mprotect(ALIGN_ADDR(origin_func), page_size,
             PROT_READ | PROT_WRITE | PROT_EXEC);
    *origin_func = (size_t)func_address;
    return PLTHOOK_SUCCESS;
  }
  return PLTHOOK_NOT_FOUND;
}

void plt_hook_debug(plthook_info *info) {
  printf(".symtab %p\n.strtab %p %lu\n.rela %p %lu\n", (void *)info->dynsym,
         (void *)info->str_table, info->str_sz, (void *)info->rel,
         info->rel_cnt);
}