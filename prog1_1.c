#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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
	
	printf("Assignment #1-1, Reian Festejo, rfestejo@sdsu.edu\n");
	int len;
	unsigned char* buff;
	unsigned int crc32;	
	FILE *fp = fopen(argv[1], "rb");
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	buff = (unsigned char*) malloc(len);
	fread(buff,len,1,fp);
	crc32 = crc(buff,len);
	printf("%d\n", crc32);
	printf("%0x\n", crc32);

	return 0;
}