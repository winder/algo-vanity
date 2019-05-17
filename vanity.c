#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "tweetnacl.h"

const char *rfc4648_alphabet =  "abcdefghijklmnopqrstuvwxyz234567";

int base32(uint32_t *firstbits, uint32_t *bitmask, const char *firstletters, size_t len){
	*firstbits = 0;
	*bitmask = 0;
	if (len > 6) {
		return 1;
	}
	for (int i = 0; i < len; i++) {
		char *c = strchr(rfc4648_alphabet, tolower((unsigned char) firstletters[i]));
		if (c != NULL) {
			uint8_t d = (uint8_t)(c - rfc4648_alphabet);
			*firstbits |= (d << (32 - 5*(i+1)));
			*bitmask |= (0x1f << (32 - 5*(i+1)));
		}
	}
	return 0;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Usage: %s PREFIX\n\twhere PREFIX is the desired five-letter-or-less prefix of the base32-encoded public key\n", argv[0]);
		printf("\tPublic and secret key will be written to /tmp/pk and /tmp/sk, respectively.\n");
		return -1;
	}
	uint32_t firstbits = 0;
	uint32_t bitmask = 0;
	base32(&firstbits, &bitmask, argv[1], strlen(argv[1]));


	unsigned char key[64] = {0};
	unsigned char pk[32] = {0};
	printf("Generating...");
	do {
		crypto_sign_keypair(pk, key);
		printf(".");
	} while (((((uint32_t)pk[0]<<24)|((uint32_t)pk[1]<<16)|((uint32_t)pk[2]<<8)|((uint32_t)pk[3])) & bitmask) != (firstbits & bitmask));
	printf("\nDone! Public and secret key will be written to /tmp/pk and /tmp/sk, respectively.\n");
	//printf("%02x%02x%02x%02x & %04x == %04x",pk[0],pk[1],pk[2],pk[3], bitmask, firstbits);

	int hamming_weight = 0;
	for (; bitmask != 0; hamming_weight += (bitmask & 1), bitmask >>= 1) {}
	printf("Your key has %d fewer bits of entropy because of the prefix.\n", hamming_weight);

	int fd;
	fd = open("/tmp/sk", O_WRONLY|O_CREAT, 0600);
	write(fd, key, 64);
	close(fd);
	fd = open("/tmp/pk", O_WRONLY|O_CREAT, 0666);
	write(fd, pk, 32);
	close(fd);
	for (int i = 0; i < 64; i++) {
		*((volatile unsigned char*)(key+i)) = 0;
	}
	for (int i = 0; i < 32; i++) {
		*((volatile unsigned char*)(pk+i)) = 0;
	}
	return 0;
}
