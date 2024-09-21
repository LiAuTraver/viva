#pragma once
// enable the test framework
#include <stdio.h>
#include <signal.h>
#define VIVA_TEST_FRAMEWORK_ENABLED 1
#ifndef _DEBUG
#define _DEBUG 1
#endif
#include "viva.h"
#define VIVA_TEST_SETUP() \
	void signal_handler(int signum) { \
		fprintf(stderr,"Signal %d received.\n", signum); \
		fflush(stderr); \
		VIVA_RUNTIME_DEBUG_RAISE \
	} \
	int main() { \
		fprintf(stdout,"Running tests...\n"); \
		signal(SIGSEGV , signal_handler); \
		int failed = 0; \
		int passed = 0; \
		int total = 0;

#define VIVA_TEST_FINISH() \
		fprintf(stdout,"Tests finished.\n"); \
		fprintf(stdout,"Total: %d, Passed: %d, Failed: %d\n", total, passed, failed); \
		fflush(stdout); \
		return failed; \
	}

#define VIVA_EXPECT_EQ(x,y) \
		VIVA_RUNTIME_REQUIRE(x,y)
#define VIVA_EXPECT_TRUE(x) \
		VIVA_RUNTIME_REQUIRE(x)

#ifndef VIVA_EXPORT_OFF
#define EXPECT_EQ(x,y) VIVA_EXPECT_EQ(x,y)
#define EXPECT_TRUE(x) VIVA_EXPECT_TRUE(x)
#define TEST_SETUP() VIVA_TEST_SETUP()
#define TEST_FINISH() VIVA_TEST_FINISH()
#endif