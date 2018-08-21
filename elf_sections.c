#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include "elf_sections.h"

// typedef struct{ 
	// Elf32_Word sh_name;//小节名在字符表中的索引 
	// E1t32_Word sh_type;//小节的类型 
	// Elf32_Word sh_flags;//小节属性 
	// Elf32_Addr sh_addr; //小节在运行时的虚拟地址 
	// Elf32_Off sh_offset;//小节的文件偏移 
	// Elf32_Word sh_size;//小节的大小．以字节为单位 
	// Elf32_Word sh_link;//链接的另外一小节的索引 
	// Elf32 Word sh_info;//附加的小节信息 
	// Elf32 Word sh_addralign;//小节的对齐 
	// Elf32 Word sh_entsize; //一些sections保存着一张固定大小入口的表。就像符号表 
// }Elf32_Shdr; 


void SegmentInfo(void)
{
	char buf[32];
	memset(buf,0,sizeof(buf));

	printf("[nr]   sh_name   sh_type   sh_flags  sh_addr   sh_offset  sh_size   sh_link  sh_info sh_addralign  sh_entsize\n");
}
void read_section_name(char *strtab, int idex,char *name)
{
	int i=0;
	char *str=&strtab[idex];
	while((*str)!='\0'){
		name[i]=*str++;
		i++;
	}
	if(i==0){
	   memcpy(name,"NULL",strlen("NULL"));
	}
	return ;
}
char * read_section_flag(int flag)
{
	switch(flag){

		case SHF_WRITE:
			return "W";
		case SHF_ALLOC:
			return "A";
		case SHF_EXECINSTR:
			return "X";
		case SHF_ALLOC|SHF_EXECINSTR:
			return "AX";
		case SHF_ALLOC|SHF_WRITE:
			return "AW";
		case SHF_EXECINSTR|SHF_WRITE:
			return "WX";
		case  SHF_MASKPROC:
			return "M";
		default:
			return "null";
	}
}
char * read_section_type(int type)
{
	switch(type){
        case SHT_NULL:        return "NULL";
        case SHT_PROGBITS:        return "PROGBITS";
        case SHT_SYMTAB:        return "SYMTAB";
        case SHT_STRTAB:        return "STRTAB";
        case SHT_RELA:        return "RELA";
        case SHT_HASH:        return "HASH";
        case SHT_DYNAMIC:        return "DYNAMIC";
        case SHT_NOTE:        return "NOTE";
        case SHT_NOBITS:        return "NOBITS";
        case SHT_REL:        return "REL";
        case SHT_SHLIB:        return "SHLIB";
        case SHT_DYNSYM:        return "DYNSYM";
        case SHT_INIT_ARRAY:    return "INIT_ARRAY";
        case SHT_FINI_ARRAY:    return "FINI_ARRAY";
        case SHT_PREINIT_ARRAY:    return "PREINIT_ARRAY";
        case SHT_GNU_HASH:        return "GNU_HASH";
        case SHT_GROUP:        return "GROUP";
        case SHT_SYMTAB_SHNDX:    return "SYMTAB SECTION INDICIES";
        case SHT_GNU_verdef:    return "VERDEF";
        case SHT_GNU_verneed:    return "VERNEED";
        case SHT_GNU_versym:    return "VERSYM";
        case 0x6ffffff0:        return "VERSYM";
        case 0x6ffffffc:        return "VERDEF";
        case 0x7ffffffd:        return "AUXILIARY";
        case 0x7fffffff:        return "FILTER";
        case SHT_GNU_LIBLIST:    return "GNU_LIBLIST";
        default:return "unknown";
	}
}


void sectionProcess(char * strtab,Elf32_Shdr *SegHeader,int index)
{
	char buf[32];
	memset(buf,0,sizeof(buf));
	read_section_name(strtab,SegHeader->sh_name,buf);
	printf("[%2d] ",index );
	printf("%-18s ", buf);
	printf("%-16s ", read_section_type(SegHeader->sh_type));
	printf("%-8s  ", read_section_flag(SegHeader->sh_flags));
	printf("%08x  ", SegHeader->sh_addr);
	printf("%08x  ", SegHeader->sh_offset);
	printf("%08x  ", SegHeader->sh_size);
	printf("%08x  ", SegHeader->sh_link);
	printf("%08x  ", SegHeader->sh_info);
	printf("%08x  ", SegHeader->sh_addralign);
	printf("%08x  ", SegHeader->sh_entsize);
	printf("\n");
}
//get section string table

void get_section_strtab(FILE *fp,Elf32_Ehdr *elf_header ,Elf32_Shdr *SegHeader,char * shstrtab)
{
	fseek(fp,SegHeader[elf_header->e_shstrndx].sh_offset,0); 
	fread(shstrtab,1,SegHeader[elf_header->e_shstrndx].sh_size,fp);
}

void get_sym_strtab(FILE *fp,Elf32_Shdr *SegHeader,char *symtab)
{
	int i=0;
	fseek(fp,SegHeader->sh_offset,SEEK_SET); 
	fread(symtab,1,SegHeader->sh_size,fp);	
}

void get_sections(FILE *fp,Elf32_Shdr *SegHeader ,int size,int offset)
{
	fseek(fp,offset,SEEK_SET);
	fread(SegHeader,sizeof(Elf32_Shdr),size,fp);
}
int  get_section_index(Elf32_Shdr *SegHeader,int size,char *src_str,char *dest_str)
{
	int i=0;
	int ret=0;
	for(i=0;i<size;i++)
	{
		char buf[32];
		memset(buf,0,sizeof(buf));
		read_section_name(src_str,SegHeader[i].sh_name,buf);
		if(strcmp(buf,dest_str)==0){
			ret = i;
			break;
		}
	}
	return ret;
}

int get_section_context(FILE *fp,Elf32_Shdr *SegHeader,char *buf,int size)
{
	int ret = fseek(fp,SegHeader->sh_offset,SEEK_SET);
	if(ret<0){
		return -1;
	}
	ret = fread(buf,1,size,fp);
	return ret;
}

int  change_section_context(FILE *fp,Elf32_Shdr *SegHeader,char *buf,int size)
{
	int ret = fseek(fp,SegHeader->sh_offset,SEEK_SET);
	if(ret<0){
		return -1;
	}
	ret = fwrite(buf,1,size,fp);
	return ret;
}
