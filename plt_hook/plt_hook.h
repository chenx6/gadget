#ifndef _PLT_HOOK_H_
#define _PLT_HOOK_H_

#include <elf.h>
#include <stddef.h>

typedef struct _plthook_info {
  // Base Address of library
  uint8_t *base_addr;
  // Address of symbol table
  Elf64_Sym *dynsym;
  // Address of string table
  char *str_table;
  size_t str_sz;
  // Address of relocation entries associated solely with the PLT
  Elf64_Rela *rel;
  size_t rel_cnt;
} plthook_info;

typedef enum _plthook_status {
  PLTHOOK_SUCCESS = 0,
  PLTHOOK_ARGUMENT_ERROR = 1,
  // plt_hook_init
  PLTHOOK_DLOPEN_ERROR,
  PLTHOOK_DLINFO_ERROR,
  PLTHOOK_SYMTAB_ERROR,
  PLTHOOK_REL_ERROR,
  PLTHOOK_STR_ERROR,
  // plt_hook_replace
  PLTHOOK_NOT_FOUND,
  // ?
  PLTHOOK_UNKNOWN,
  PLTHOOK_MAX,
} plthook_status;

plthook_status plt_hook_init(plthook_info *info, char *name);
plthook_status plt_hook_replace(plthook_info *info, char *name, void *func_address);
void plt_hook_debug(plthook_info *info);

#endif