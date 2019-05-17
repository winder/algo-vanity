vanity: vanity.c tweetnacl.c tweetnacl.h random.c
	gcc -O3 -lbsd -o vanity random.c tweetnacl.c vanity.c
