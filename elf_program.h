#ifndef _ELF_PROGRAM_H_
#define _ELF_PROGRAM_H_
#include <elf.h>
void program_header_Process(Elf32_Phdr *pro_header);
void ger_programs(FILE *fp ,Elf32_Phdr *pro_header,int size);

#endif