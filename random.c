#include <bsd/stdlib.h>
void randombytes(unsigned char *buf, unsigned long long len) {
	arc4random_buf(buf, len);
}
