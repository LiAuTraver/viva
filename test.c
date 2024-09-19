#include "generic.h"

void test_typename(void) {
	size_t s;
	ptrdiff_t p;
	intmax_t i;

	const int c = 0;
	int ai[3] = {0};

	printf("size_t is '%s'\n", reflexpr(s));
	printf("ptrdiff_t is '%s'\n", reflexpr(p));
	printf("intmax_t is '%s'\n", reflexpr(i));

	printf("character constant is '%s'\n", reflexpr('0'));
	printf("0x7FFFFFFF is '%s'\n", reflexpr(0x7FFFFFFF));
	printf("0xFFFFFFFF is '%s'\n", reflexpr(0xFFFFFFFF));
	printf("0x7FFFFFFFU is '%s'\n", reflexpr(0x7FFFFFFFU));
	printf("array of int is '%s'\n", reflexpr(ai));
}