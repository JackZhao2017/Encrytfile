#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned char key[64];
static char in_name[128];
static char out_name[128];

int process_cmdline(int argc, const char **argv);



void init_key(void)
{
    memset(key,0x01,sizeof(key));
    /*
        或者自定义key
    */
    FILE *fp=fopen("key","wb");

    fwrite(key,1,sizeof(key),fp);

    fclose(fp);
}

int encrypt_data(unsigned char  *in ,unsigned char *out,int len)
{
    for(int i=0;i<len;i++)
    {
        out[i]=in[i]^key[i];
    }
    return 0;
}

int decrypt_data(unsigned char * in ,unsigned char *out,int len)
{

    for(int i=0;i<len;i++)
    {
        out[i]=in[i]^key[i];
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    process_cmdline(argc,argv);
    
    FILE *fp=fopen(argv[1],"r");
    if(fp==NULL)
    {
        printf("cannot open in file %s \n",argv[1]);
        return -1;
    }
    FILE *fw=fopen(argv[2],"wb");
    if(fw==NULL)
    {
        printf("cannot open out file %s \n",argv[2]);
        goto err;
    } 

    init_key();

    while(!feof(fp)){
        
        unsigned char data[64];
        unsigned char encryptdata[64];

        memset(data,0,sizeof(data));
        memset(encryptdata,0,sizeof(encryptdata));

        int readlen = fread(data,1,sizeof(data),fp);  

        encrypt_data(data,encryptdata,readlen);

        int writelen=fwrite(encryptdata,1,readlen,fw);

        if(readlen!=writelen){
            printf("encrypt data length wrong\n");
            break;
        }
    }


err:
    fclose(fp);
    fclose(fw);
    return 0;
}

int process_cmdline(int argc, const char **argv)
{

    int i;

    for (i = 1; i < argc; i++) {

        if(strcmp(argv[i], "-i") == 0){
            memset(in_name,0,sizeof(in_name));
            //memcpy(in_name,argv[++i],strlen(argv[i]));
            sprintf(in_name,"%s\n",argv[++i]);
            printf("in %s %s \n",argv[i] ,in_name );
        }
        if(strcmp(argv[i], "-o") == 0){
            memset(out_name,0,sizeof(out_name));
            sprintf(out_name,"%s\n",argv[++i]);
            printf("out %s %s \n",argv[i] ,out_name );          
        }
    }
    return 0;
}





