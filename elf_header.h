#ifndef _ELF_HEADER_H_
#define _ELF_HEADER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <elf.h>

void elf_headerprocess(Elf32_Ehdr elf_header);

void get_elfheader(FILE *fp,Elf32_Ehdr *elf_header);

void change_elfheader(FILE *fp,Elf32_Ehdr elf_header);
#ifdef __cplusplus
};
#endif
#endif