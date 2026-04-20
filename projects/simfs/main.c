#include <stdio.h>


extern int simfs(char const *const diskpath);

int main(void) {
	// in debugging process the stdout won't flush, make it flush.
	setvbuf(stdout, nullptr, _IONBF, 0);
	setvbuf(stderr, nullptr, _IONBF, 0);
	return simfs("M:\\Projects\\viva\\projects\\simfs\\vdisk.bin");
}
