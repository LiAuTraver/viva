#pragma once
#define VIVA_TEST_FRAMEWORK_ENABLED 1
#include <stdio.h>
#include "viva.h"
#define VIVA_TEST_SETUP() \
		int main() { \
				fprintf(stdout,"Running tests...\n"); \
				int failed = 0; \
				int passed = 0; \
				int total = 0;

#define VIVA_TEST_FINISH() \
		fprintf(stdout,"Tests finished.\n"); \
		fprintf(stdout,"Total: %d, Passed: %d, Failed: %d\n", total, passed, failed); \
		return failed; \
	}

#define VIVA_EXPECT_EQ(x,y) \
		VIVA_RUNTIME_REQUIRE(x,y)

#define EXPECT_EQ(x,y) VIVA_EXPECT_EQ(x,y)