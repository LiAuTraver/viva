#define VIVA_DEBUG_ENABLED 1
#if _MSC_VER
#define CRT_SECURE_NO_WARNINGS 1
#endif

#include <stdio.h>
#include <string.h>
#include "include/net/ancillarycat/viva/viva.h"

int main() {
	var str = get_from_stdin_raw(any.char_ptr_type, 100);
	println(str);

	return 0;
}

