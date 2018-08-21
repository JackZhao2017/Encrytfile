#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include "elf_sym.h"
#include <elf.h>

// typedef struct {
//   Elf32_Word st_name;
//   Elf32_Addr st_value;
//   Elf32_Word st_size;
//   unsigned char st_info;
//   unsigned char st_other;
//   Elf32_Half st_shndx;
// } Elf32_Sym;


void read_sym_name(const char *syntab,int idex,char *name,int size)
{
	int i=0;
	char *str=syntab+idex;
	while(*(str+i)!='\0'){
		name[i]=*(str+i);
		i++;
		if(i==size){
			break;
		}
	}
	if(i==0){
	   memcpy(name,"NULL",strlen("NULL"));
	}
	return ;
}
char * read_sym_bind(int bind)
{
	switch(bind){
		case STB_LOCAL:return "LOCAL";
		case STB_GLOBAL:return "GLOBAL";
		case STB_WEAK:return "WEAK";
	}
}
char * read_sym_type(int type)
{
	switch(type){
		case STT_NOTYPE:return "NOTYPE";
		case STT_OBJECT:return "OBJECT";
		case STT_FUNC:return "FUNC";
		case STT_SECTION:return "SECTION";
		case STT_FILE:return "FILE";
	}
}
void print_sym_ndx(int ndx)
{
	switch(ndx){
		case SHN_ABS:
			printf("%-3s ","ABS");
			return;
		case SHN_COMMON:
			printf("%-3s ","COM");
			return ;
		case SHN_UNDEF:
			printf("%-3s ","UND");
			return ;
		default:
			printf("%3d ", ndx);
			return ;
	}
}
void print_sym_info(const char *str, Elf32_Sym  *Sym)
{
	char buf[32];
	memset(buf,0,sizeof(buf));
	read_sym_name(str  ,Sym->st_name,buf,sizeof(buf));
	printf("%08x ",Sym->st_value );
	printf("%8d  ",Sym->st_size );
	printf("%-8s ",read_sym_type(Sym->st_info&0xf));
	printf("%-8s ",read_sym_bind(Sym->st_info>>4));
	print_sym_ndx(Sym->st_shndx);
	printf("%8d  ",Sym->st_name );
	printf("%-8s ",buf);
	printf("\n");
}


int get_sym_index(const char *str,Elf32_Sym  *Sym,int num,const char *destname)
{
	int i=0;
	char buf[128];
	int ret=0;
	printf(" num %d \n", num);
	for(i=0;i<num;i++){
		memset(buf,0,sizeof(buf));
		read_sym_name(str ,Sym[i].st_name,buf,sizeof(buf));
		if(strcmp(destname,buf)==0){
			ret=i;
			break;
		}
		if(strstr(buf,destname)!=NULL){
			printf("%s\n",buf);
			ret=i;
			break;
		}		
	}
	return ret;
}









