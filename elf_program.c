#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "elf_program.h"

#define PT_NULL         0               /* Program header table entry unused */
#define PT_LOAD         1               /* Loadable program segment */
#define PT_DYNAMIC      2               /* Dynamic linking information */
#define PT_INTERP       3               /* Program interpreter */
#define PT_NOTE         4               /* Auxiliary information */
#define PT_SHLIB        5               /* Reserved */
#define PT_PHDR         6               /* Entry for header table itself */
#define PT_TLS          7               /* Thread-local storage segment */
#define PT_NUM          8               /* Number of defined types */
#define PT_LOOS         0x60000000      /* Start of OS-specific */
#define PT_GNU_EH_FRAME 0x6474e550      /* GCC .eh_frame_hdr segment */
#define PT_GNU_STACK    0x6474e551      /* Indicates stack executability */
#define PT_GNU_RELRO    0x6474e552      /* Read-only after relocation */
#define PT_LOSUNW       0x6ffffffa
#define PT_SUNWBSS      0x6ffffffa      /* Sun Specific segment */
#define PT_SUNWSTACK    0x6ffffffb      /* Stack segment */
#define PT_HISUNW       0x6fffffff
#define PT_HIOS         0x6fffffff      /* End of OS-specific */
#define PT_LOPROC       0x70000000      /* Start of processor-specific */
#define PT_HIPROC       0x7fffffff      /* End of processor-specific */


char *get_program_type(int type)
{
	switch(type){
		case PT_NULL:return "NULL";
		case PT_LOAD:return "LOAD";
		case PT_DYNAMIC:return "DYNAMIC";
		case PT_INTERP:return "PT_INTERP";
		case PT_NOTE:return "NOTE";
		case PT_GNU_EH_FRAME:return "GNU_EH_FRAME";
		case PT_GNU_STACK:return "GNU_STACK";
		case PT_GNU_RELRO:return "GNU_RELRO";
	}
}


void program_header_Process(Elf32_Phdr *pro_header)
{
	printf("%-16s ",get_program_type(pro_header->p_type) );
	printf("0x%08x ",pro_header->p_offset);
	printf("0x%08x ",pro_header->p_vaddr);
	printf("0x%08x ",pro_header->p_paddr);
	printf("0x%08x ",pro_header->p_filesz);
	printf("0x%08x ",pro_header->p_memsz);
	printf("0x%-4x ",pro_header->p_flags);
	printf("0x%-4x ",pro_header->p_align);

	printf("\n");
}

void ger_programs(FILE *fp ,Elf32_Phdr *pro_header,int size)
{
	fseek(fp,52,SEEK_SET);
	fread(pro_header,sizeof(Elf32_Phdr),size,fp); 
}