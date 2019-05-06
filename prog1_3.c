#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#pragma pack(push,1)
#pragma pack(pop)

#define EI_NIDENT 16

/* 64-bit ELF base types. */
typedef unsigned long long Elf64_Addr;
typedef unsigned short Elf64_Half;
typedef signed short Elf64_SHalf;
typedef unsigned long long Elf64_Off;
typedef signed int Elf64_Sword;
typedef unsigned int Elf64_Word;
typedef unsigned long long Elf64_Xword;
typedef signed long long Elf64_Sxword;


typedef struct elf64_hdr {
	unsigned char e_ident[EI_NIDENT]; /* ELF "magic number" */
	Elf64_Half e_type;
	Elf64_Half e_machine;
	Elf64_Word e_version;
	Elf64_Addr e_entry;       /* Entry point virtual address */
	Elf64_Off e_phoff;        /* Program header table file offset */
	Elf64_Off e_shoff;        /* Section header table file offset */
	Elf64_Word e_flags;
	Elf64_Half e_ehsize;
	Elf64_Half e_phentsize;
	Elf64_Half e_phnum;
	Elf64_Half e_shentsize;
	Elf64_Half e_shnum;
	Elf64_Half e_shstrndx;
} Elf64_Ehdr;

unsigned long crc_table[256];

int crc_table_computed = 0;

void make_crc_table(void) {
	unsigned long c;
	int n, k;

	for (n = 0; n < 256; n++) {
		c = (unsigned long) n;
		for (k = 0; k < 8; k++) {
			if (c & 1)
				c = 0xEDB88320L ^ (c >> 1);
			else
				c = c >> 1;
		}
		crc_table[n] = c;
	}
	crc_table_computed = 1;
}

unsigned long update_crc(unsigned long crc, unsigned char *buf, int len) {
	unsigned long c = crc;
	int n;

	if (!crc_table_computed)
		make_crc_table();
	for (n = 0; n < len; n++) {
		c = crc_table[(c ^ buf[n]) & 0xFF] ^ (c >> 8);
	}
	return c;
}

unsigned long crc(unsigned char *buf, int len) {
	return update_crc(0xFFFFFFFFL, buf, len) ^ 0xFFFFFFFFL;
}

int main( int argc, char *argv[] ){
	int len;
	unsigned char* buff;
	Elf64_Ehdr elfhead; 
	FILE* ElfFile = NULL;
	char* SectNames = NULL;
	Elf64_Ehdr elfHdr;
	Elf64_Shdr sectHdr;
	uint32_t idx;

	if(argc != 2) {
		printf("usage: %s <ELF_FILE>\n", argv[0]);
		exit(1);
	}

	if((ElfFile = fopen(argv[1], "r")) == NULL) {
		perror("[E] Error opening file:");
		exit(1);
	}

	fread(&elfHdr, 1, sizeof(Elf64_Ehdr), ElfFile);

	fseek(ElfFile, elfHdr.e_shoff + elfHdr.e_shstrndx * sizeof(sectHdr), SEEK_SET);
	fread(&sectHdr, 1, sizeof(sectHdr), ElfFile);
	SectNames = malloc(sectHdr.sh_size);
	fseek(ElfFile, sectHdr.sh_offset, SEEK_SET);
	fread(SectNames, 1, sectHdr.sh_size, ElfFile);

	for (idx = 0; idx < elfHdr.e_shnum; idx++)
	{
		const char* name = "";

		fseek(ElfFile, elfHdr.e_shoff + idx * sizeof(sectHdr), SEEK_SET);
		fread(&sectHdr, 1, sizeof(sectHdr), ElfFile);

		if (sectHdr.sh_name);
		name = SectNames + sectHdr.sh_name;
		printf("%2u %s\n", idx, name);
	}

	crc32 = crc(buff,len);
	printf("%0x\n", crc32);
	fclose(fp);
	fclose(out);

	return 0;
}
