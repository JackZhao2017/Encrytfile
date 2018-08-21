#ifndef _ELF_SECTIONS_H_
#define _ELF_SECTIONS_H_

#include <elf.h>
int  get_section_context(FILE *fp,Elf32_Shdr *SegHeader,char *buf,int size);
void sectionProcess(char * strtab,Elf32_Shdr *SegHeader,int index);
void get_sections(FILE *fp,Elf32_Shdr *SegHeader ,int size,int offset);
int  get_section_index(Elf32_Shdr *SegHeader,int size,char *src_str,char *dest_str);
int  change_section_context(FILE *fp,Elf32_Shdr *SegHeader,char *buf,int size);
void get_section_strtab(FILE *fp,Elf32_Ehdr *elf_header ,Elf32_Shdr *SegHeader,char * shstrtab);
void get_sym_strtab(FILE *fp,Elf32_Shdr *SegHeader,char *symtab);
#endif