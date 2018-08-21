#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include <elf.h>

void read_elf_typedefine(void)
{
	printf("Elf32_Half  %d \n",sizeof(Elf32_Half));
	printf("Elf32_Word   %d \n",sizeof(Elf32_Word));
	printf("Elf32_Addr   %d \n",sizeof(Elf32_Addr));
	printf("Elf32_Off   %d \n",sizeof(Elf32_Off));	

	printf("EI_NIDENT %d \n",EI_NIDENT);
	printf("EI_CLASS %d \n",EI_CLASS );
}
char * read_elf_class(int elf_class)
{
    if(ELFCLASSNONE==elf_class) return "none";
    if(ELFCLASS32==elf_class)  return "ELF32";
    if(ELFCLASS64==elf_class) return "ELF64";
}
char * read_data_encoding(int encoding){
    switch (encoding)
    {
        case ELFDATANONE: return ("none");
        case ELFDATA2LSB: return ("2's complement, little endian");
        case ELFDATA2MSB: return ("2's complement, big endian");
        default:
            return "<unknown:>";
    }
}
const char *read_osabi_name (unsigned int osabi){

    switch (osabi){
     case  ELFOSABI_NONE:        return "UNIX System V ABI";
     case  ELFOSABI_HPUX:        return "HP-UX";
     case  ELFOSABI_NETBSD:      return "NetBSD";
     case  ELFOSABI_GNU:         return "Object uses GNU ELF extensions";
     case  ELFOSABI_SOLARIS:     return "Sun Solaris";
     case  ELFOSABI_AIX:         return "IBM AIX";
     case  ELFOSABI_IRIX:        return "SGI Irix";
     case  ELFOSABI_FREEBSD:     return "FreeBSD";
     case  ELFOSABI_TRU64:       return "Compaq TRU64 UNIX";
     case  ELFOSABI_MODESTO:     return "Novell Modesto";
     case  ELFOSABI_OPENBSD:     return "OpenBSD";
     case  ELFOSABI_ARM_AEABI:   return "ARM EABI";
     case  ELFOSABI_ARM:         return "ARM";
     case  ELFOSABI_STANDALONE:  return "Standalone (embedded) application";
        default:
            break;
    }
    return "unknown";
}

char * read_file_type(int e_type){

	static char buff[32];
    switch (e_type) {
        case ET_NONE:
            return "NONE (None)";
        case ET_REL:
            return "REL (Relocatable file)";
        case ET_EXEC:
            return "EXEC (Executable file)";
        case ET_DYN:
            return "DYN (Shared object file)";
        case ET_CORE:
            return "CORE (Core file)";

        default:
            if ((e_type >= ET_LOPROC) && (e_type <= ET_HIPROC))
                snprintf(buff, sizeof(buff), ("Processor Specific: (%x)"), e_type);
            else if ((e_type >= ET_LOOS) && (e_type <= ET_HIOS))
                snprintf(buff, sizeof(buff), ("OS Specific: (%x)"), e_type);
            else
                snprintf(buff, sizeof(buff), ("<unknown>: %x"), e_type);
            return buff;

    }
}
const char *read_machine_name(int  e_machine) {


    switch (e_machine){
        case EM_NONE:        return ("None");
        // case EM_AARCH64:    return "AArch64";
        case EM_M32:        return "WE32100";
        case EM_SPARC:        return "Sparc";
        case EM_386:        return "Intel 80386";
        case EM_68K:        return "MC68000";
        case EM_88K:        return "MC88000";
        case EM_860:        return "Intel 80860";
        case EM_MIPS:        return "MIPS R3000";
        case EM_S370:        return "IBM System/370";
        case EM_MIPS_RS3_LE:    return "MIPS R4000 big-endian";
        case EM_PARISC:        return "HPPA";
        case EM_SPARC32PLUS:    return "Sparc v8+" ;
        case EM_960:        return "Intel 90860";
        case EM_PPC:        return "PowerPC";
        case EM_PPC64:        return "PowerPC64";
        case EM_FR20:        return "Fujitsu FR20";
        case EM_RH32:        return "TRW RH32";
        case EM_ARM:        return "ARM";
        case EM_SH:            return "Renesas / SuperH SH";
        case EM_SPARCV9:        return "Sparc v9";
        case EM_TRICORE:        return "Siemens Tricore";
        case EM_ARC:        return "ARC";
        case EM_H8_300:        return "Renesas H8/300";
        case EM_H8_300H:        return "Renesas H8/300H";
        case EM_H8S:        return "Renesas H8S";
        case EM_H8_500:        return "Renesas H8/500";
        case EM_IA_64:        return "Intel IA-64";
        case EM_MIPS_X:        return "Stanford MIPS-X";
        case EM_COLDFIRE:        return "Motorola Coldfire";
        case EM_ALPHA:        return "Alpha";
        case EM_D10V:        return "d10v";
        case EM_D30V:        return "d30v";
        case EM_M32R:        return "Renesas M32R (formerly Mitsubishi M32r)";
        case EM_V800:        return "Renesas V850 (using RH850 ABI)";
        case EM_V850:        return "Renesas V850";
        case EM_MN10300:        return "mn10300";
        case EM_MN10200:        return "mn10200";
        case EM_FR30:        return "Fujitsu FR30";
        case EM_PJ:            return "picoJava";
        case EM_MMA:        return "Fujitsu Multimedia Accelerator";
        case EM_PCP:        return "Siemens PCP";
        case EM_NCPU:        return "Sony nCPU embedded RISC processor";
        case EM_NDR1:        return "Denso NDR1 microprocesspr";
        case EM_STARCORE:        return "Motorola Star*Core processor";
        case EM_ME16:        return "Toyota ME16 processor";
        case EM_ST100:        return "STMicroelectronics ST100 processor";
        case EM_TINYJ:        return "Advanced Logic Corp. TinyJ embedded processor";
        case EM_PDSP:        return "Sony DSP processor";
        case EM_FX66:        return "Siemens FX66 microcontroller";
        case EM_ST9PLUS:        return "STMicroelectronics ST9+ 8/16 bit microcontroller";
        case EM_ST7:        return "STMicroelectronics ST7 8-bit microcontroller";
        case EM_68HC16:        return "Motorola MC68HC16 Microcontroller";
        case EM_68HC12:        return "Motorola MC68HC12 Microcontroller";
        case EM_68HC11:        return "Motorola MC68HC11 Microcontroller";
        case EM_68HC08:        return "Motorola MC68HC08 Microcontroller";
        case EM_68HC05:        return "Motorola MC68HC05 Microcontroller";
        case EM_SVX:        return "Silicon Graphics SVx";
        case EM_ST19:        return "STMicroelectronics ST19 8-bit microcontroller";
        case EM_VAX:        return "Digital VAX";
        case EM_AVR:        return "Atmel AVR 8-bit microcontroller";
        case EM_CRIS:        return "Axis Communications 32-bit embedded processor";
        case EM_JAVELIN:        return "Infineon Technologies 32-bit embedded cpu";
        case EM_FIREPATH:        return "Element 14 64-bit DSP processor";
        case EM_ZSP:        return "LSI Logic‘s 16-bit DSP processor";
        case EM_MMIX:        return "Donald Knuth‘s educational 64-bit processor";
        case EM_HUANY:        return "Harvard Universitys‘s machine-independent object format";
        case EM_PRISM:        return "Vitesse Prism";
        case EM_X86_64:        return "Advanced Micro Devices X86-64";
        case EM_S390:        return "IBM S/390";
        // case EM_OPENRISC:
        case EM_ARC_A5:        return "ARC International ARCompact processor";
        case EM_XTENSA:        return "Tensilica Xtensa Processor";
        // case EM_MICROBLAZE:
        // case EM_TILEPRO:        return "Tilera TILEPro multicore architecture family";
       // case EM_TILEGX:        return "Tilera TILE-Gx multicore architecture family";
    }

    return "ubknow";


}
void elf_headerprocess(Elf32_Ehdr elf_header)
{
	int i=0;

	printf("ELF Header:\n");
	
	printf("Elf32_Ehdr %d \n",sizeof(Elf32_Ehdr));

    for ( i = 0; i <EI_NIDENT ; ++i)
        printf ("%2.2x ", elf_header.e_ident[i]);
        printf("\n");

    printf("  Class:                             %s\n",
            read_elf_class(elf_header.e_ident[EI_CLASS]));

    printf ("  Data:                              %s\n",
            read_data_encoding (elf_header.e_ident[EI_DATA]));

    printf ("  Version:                           %d %s\n",
            elf_header.e_ident[EI_VERSION],
            (elf_header.e_ident[EI_VERSION] == EV_CURRENT
             ? "(current)"
             : (elf_header.e_ident[EI_VERSION] != EV_NONE
                ? ("<unknown: %lx>")
                : "")));

    printf ("  OS/ABI:                            %s\n",
            read_osabi_name (elf_header.e_ident[EI_OSABI]));

    printf ("  ABI Version:                       %d\n",
            elf_header.e_ident[EI_ABIVERSION]);

    printf ("  Type:                              %s\n",
            read_file_type (elf_header.e_type));

    printf ("  Machine:                           %s\n",
            read_machine_name (elf_header.e_machine));

    printf ("  Version:                           0x%lx\n",
            (unsigned long) elf_header.e_version);

    printf ("  Entry point address:               0x%x",elf_header.e_entry);

    printf ("\n  Start of program headers:          %d",elf_header.e_phoff);

    printf (" (bytes into file)\n  Start of section headers:          %d",elf_header.e_shoff);
    printf (" (bytes into file)\n");

    printf ("  Flags:                             0x%lx\n",(unsigned  long)elf_header.e_flags);

    printf ("  Size of this header:               %ld (bytes)\n",(long)elf_header.e_ehsize);

    printf ("  Size of program headers:           %ld (bytes)\n",(long)elf_header.e_phentsize);

    printf ("  Number of program headers:         %ld\n",(long)elf_header.e_phnum);



    printf ("  Size of section headers:           %ld (bytes)\n",
            (long) elf_header.e_shentsize);

    printf ("  Number of section headers:         %ld\n",
            (long) elf_header.e_shnum);


    printf ("  Section header string table index: %ld\n",
            (long) elf_header.e_shstrndx);

}

void get_elfheader(FILE *fp,Elf32_Ehdr *elf_header)
{
    fread(elf_header ,sizeof(Elf32_Ehdr),1,fp);
}

void change_elfheader(FILE *fp,Elf32_Ehdr elf_header)
{
    fseek(fp,0,SEEK_SET);
    fwrite(&elf_header,sizeof(Elf32_Ehdr),1,fp);
}



