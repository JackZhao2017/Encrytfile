#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <unistd.h>

typedef struct elf
{
	Elf32_Ehdr elf_header;
	Elf32_Phdr *program;
	int keysize;
	int sosize;
	int loadersize;
	int virual_addr;
	FILE *floader;
	FILE *fkey;
	FILE *fso;
	FILE *fsow;
}ELF_t;

static char in_name[128];
static char out_name[128];
int process_cmdline(int argc, const char **argv);
void GetLoaderProgram(ELF_t *elf)
{
	FILE *fp=elf->floader;
	if(elf->program==NULL)
		return;
	fread(elf->program,1,sizeof(Elf32_Phdr)*(elf->elf_header.e_phnum),fp);
}

void GetLoaderHeader(FILE *fp,Elf32_Ehdr *header)
{	
	fread(header,1,sizeof(Elf32_Ehdr),fp);
}	
int GetKeysize(FILE *fkey)
{
	fseek(fkey,0,SEEK_END);
	int size = ftell(fkey);
	fseek(fkey,0,SEEK_SET);
	return size;
}

int GetSosize(FILE *fso)
{
	fseek(fso,0,SEEK_END);
	int size = ftell(fso);
	fseek(fso,0,SEEK_SET);
	return size;
}

int Getloadersize(FILE *floader)
{
	fseek(floader,0,SEEK_END);
	int size = ftell(floader);
	fseek(floader,0,SEEK_SET);
	return size;	
}
void ChangeLoaderSegmentSize(ELF_t *elf )
{
	int load_index=0,i=0;
	Elf32_Phdr *program = elf->program;
	for( i=0;i<elf->elf_header.e_phnum;i++)
	{
		if(PT_LOAD ==program[i].p_type)
		{
			if(load_index==0)
			{
				load_index=1;
			}
			else if(load_index==1)
			{
				int lastfild = elf->loadersize-program[i].p_offset-program[i].p_filesz;
				elf->virual_addr = program[i].p_vaddr+program[i].p_filesz+lastfild;
				program[i].p_filesz +=elf->keysize+elf->sosize+lastfild;
				program[i].p_memsz  +=elf->keysize+elf->sosize+lastfild;
				printf("lastfild 0x%08x\n",lastfild);
				printf("virual_addr 0x%08x\n",elf->virual_addr);
				break;
			}
		}
	}
}
void ChangeLoaderHeader(ELF_t *elf)
{
	elf->elf_header.e_shoff = elf->virual_addr;
	printf("e_shoff 0x%08x\n",elf->elf_header.e_shoff);
}
int CombineWriteLoader(ELF_t *elf)
{
	unsigned char data[64];
	int writelen=0;
	int retlen = fwrite(&elf->elf_header,1,sizeof(Elf32_Ehdr),elf->fsow);
	if(retlen!=sizeof(Elf32_Ehdr))
	{
		printf("%s write header length wrong\n",__func__ );
	}
	retlen = fwrite(elf->program,sizeof(Elf32_Phdr),elf->elf_header.e_phnum,elf->fsow);
	if(retlen!=elf->elf_header.e_phnum)
	{
		printf("%s write header length wrong\n",__func__ );
	}
	while(!feof(elf->floader))
	{
		retlen = fread(data,1,sizeof(data),elf->floader);
		writelen = fwrite(data,1,retlen,elf->fsow);
		if(retlen != writelen)
		{
			printf("%s write data length wrong\n",__func__ );
			return -1;
		}
	}
	return 0;
}
int CombineWriteKey(ELF_t *elf)
{
	unsigned char data[64];
	int writelen=0;
	int retlen = 0;
	while(!feof(elf->fkey))
	{
		retlen = fread(data,1,sizeof(data),elf->fkey);
		writelen = fwrite(data,1,retlen,elf->fsow);
		if(retlen != writelen)
		{
			printf("%s write data length wrong\n",__func__ );
			return -1;
		}
	}
	return 0;
}
int CombineWriteSo(ELF_t *elf)
{
	unsigned char data[64];
	int writelen=0;
	int retlen = 0;
	while(!feof(elf->fso))
	{
		retlen = fread(data,1,sizeof(data),elf->fso);
		writelen = fwrite(data,1,retlen,elf->fsow);
		if(retlen != writelen)
		{
			printf("%s write data length wrong\n",__func__ );
			return -1;
		}
	}
	return 0;
}

int main(int argc, char const *argv[])
{
	/* code */
	ELF_t mElf;
	memset(&mElf,0,sizeof(ELF_t));

	process_cmdline(argc, argv);

	mElf.floader =fopen("libloader.so","rb");
	if(mElf.floader==NULL)
	{
		printf("not found  loader.so\n");
		return -1;
	}
	mElf.fkey=fopen("key","rb");
	if(mElf.fkey==NULL)
	{
		printf("not found key\n");
		return -1;
	}
	mElf.fso=fopen(in_name,"rb");
	if(mElf.fso==NULL)
	{
		printf("not found %s\n",in_name);
		return -1;
	}	

	mElf.fsow=fopen(out_name,"wb");
	if(mElf.fsow==NULL)
	{
		printf("can not create  %s \n",out_name);
		return -1;
	}

	mElf.keysize = GetKeysize(mElf.fkey);
	mElf.sosize  = GetSosize(mElf.fso);
	mElf.loadersize= Getloadersize(mElf.floader);
	printf("keysize %d \n",mElf.keysize );
	printf("sosize %d \n",mElf.sosize );
	printf("loadersize %d \n",mElf.loadersize );

	GetLoaderHeader(mElf.floader,&mElf.elf_header);

	mElf.program=(Elf32_Phdr*)malloc(sizeof(Elf32_Phdr)*mElf.elf_header.e_phnum);

	 
	GetLoaderProgram(&mElf);
	ChangeLoaderSegmentSize(&mElf);
	ChangeLoaderHeader(&mElf);

	CombineWriteLoader(&mElf);
	CombineWriteKey(&mElf);
	CombineWriteSo(&mElf);

	fclose(mElf.floader);
    fclose(mElf.fso);
    fclose(mElf.fkey);
    free(mElf.program);
    fclose(mElf.fsow);  

	return 0;
}
int process_cmdline(int argc, const char **argv)
{

    int i;

    for (i = 1; i < argc; i++) {

        if(strcmp(argv[i], "-i") == 0){
            memset(in_name,0,sizeof(in_name));
            sprintf(in_name,"%s",argv[++i]);
            printf("in %s %s \n",argv[i] ,in_name );
        }
        if(strcmp(argv[i], "-o") == 0){
            memset(out_name,0,sizeof(out_name));
            sprintf(out_name,"%s",argv[++i]);
            printf("out %s %s \n",argv[i] ,out_name );          
        }
    }
    return 0;
}




