#ifndef _ELF_SYM_H_
#define _ELF_SYM_H_

#include <elf.h>

void print_sym_info(const char *str, Elf32_Sym  *Sym);
int get_sym_index(const char *str,Elf32_Sym  *Sym,int num,const char *destname);
#endif