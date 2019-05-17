vanity: vanity.c tweetnacl.c tweetnacl.h random.c
	gcc -O3 -o vanity -lbsd $^
