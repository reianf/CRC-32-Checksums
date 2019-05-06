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
	long int ph_off;
	int i;
	short ph_size;
	short ph_count;
	unsigned int crc32;	
	
	ph_off = elfhead.e_phoff;
	ph_size = elfhead.e_phentsize;
	ph_count = elfhead.e_phnum;

	printf("%ld %d %d\n", ph_off, ph_size, ph_count);

	FILE *fp = fopen(argv[1], "rb");
	FILE *out = fopen("out.binary", "wb");
	fseek(fp, 0x20, SEEK_SET);
	fread(&ph_off,sizeof(ph_off),1,fp);
	fseek(fp, 0, SEEK_END);
	fread(&ph_size,sizeof(ph_size),1,fp);
	fread(&ph_count,sizeof(ph_count),1,fp);
	
	len = ph_size*ph_count;
	fseek(fp, ph_off, SEEK_SET);
	buff = (unsigned char*) malloc(len);
	fread(buff,len,1,fp);
	fwrite(buff,len,1,out);
	crc32 = crc(buff,len);
	printf("%0x\n", crc32);
	fclose(fp);
	fclose(out);

	return 0;
}
