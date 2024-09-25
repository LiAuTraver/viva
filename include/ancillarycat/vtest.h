#pragma once
// tell the compiler not to optimize the code
#ifdef __GNUC__
#pragma GCC optimize ("O0")
#elif defind(__clang__)
#pragma clang optimize off
#elif defined(_MSC_VER)
#pragma optimize("", off)
#endif
// enable the test framework
#include <stdio.h>
#include <signal.h>
#define VIVA_TEST_FRAMEWORK_ENABLED 1
#define VIVA_DEBUG_ENABLED 1
#include "viva.h"

#define VIVA_TEST_IMPL_1(name) \
		int failed = 0; \
		int passed = 0; \
		int total = 0; \
		__attribute__((constructor)) void setup() { \
			fprintf(stdout,"Running tests for %s...\n", VIVA_STRINGIFY(VIVA_CONCAT(name))); \
		} \
		__attribute__((destructor)) void finish() { \
			val out_stream = failed ? stderr : stdout; \
			fprintf(out_stream,"\n"); \
			fprintf(out_stream,"Tests finished.\n"); \
			fprintf(out_stream,"Total: %d, Passed: %d, Failed: %d\n", total, passed, failed); \
			if (failed) { \
				fprintf(out_stream,"Test %s failed.\n", VIVA_STRINGIFY(VIVA_CONCAT(name))); \
			} else \
				fprintf(out_stream,"Test %s passed.\n", VIVA_STRINGIFY(VIVA_CONCAT(name))); \
			fflush(out_stream); \
		} \
		int main(int, char**)

#define VIVA_TEST_IMPL_2(name, sub_name) \
	VIVA_TEST_IMPL_1(VIVA_CONCAT(name,.,sub_name))

#define VIVA_TEST_IMPL_0() \
	VIVA_TEST_IMPL_1(<unnamed>)
// BUG: cannot put "foo", "bar" as arguments???
// todo: fix it
#define VIVA_TEST_IMPL(...) VIVA__VFUNC_IMPL(VIVA_TEST_IMPL, VIVA__NARG__(__VA_ARGS__))( __VA_ARGS__)
#define VIVA_TEST(...) VIVA_TEST_IMPL(__VA_ARGS__)

#define VIVA_EXPECT_EQ(x,y) \
		VIVA_RUNTIME_REQUIRE(x,y)
#define VIVA_EXPECT_TRUE(x) \
		VIVA_RUNTIME_REQUIRE(x)

#ifndef VIVA_EXPORT_OFF
#define EXPECT_EQ(x,y) VIVA_EXPECT_EQ(x,y)
#define EXPECT_TRUE(x) VIVA_EXPECT_TRUE(x)
#define TEST(...) VIVA_TEST(__VA_ARGS__)
#define TEST_FINISH() VIVA_TEST_FINISH()
#endif
