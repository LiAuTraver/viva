#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "include/net/ancillarycat/viva/viva.h"
#include "include/net/ancillarycat/viva/specialized_list.h"
#include "include/net/ancillarycat/viva/generic/list.h"

int a() {
	printf("Called a\n");
	return 0;
}

int main() {
	void* b;
	b = &a;
	((int (*)())b)();

	printf("Enter an integer: ");
	int int_value = get_rec(int_value);
	printf("You entered: %d\n", int_value);

	printf("Enter a character: ");
	char char_value = get_rec(char_value);
	printf("You entered: %c\n", char_value);
	return 0;
}
