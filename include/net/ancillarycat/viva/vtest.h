#pragma once
// tell the compiler not to optimize the code
#ifdef __GNUC__
#pragma GCC optimize("O0")
#elif defined(__clang__)
#pragma clang optimize off
#elif defined(_MSC_VER)
#pragma optimize("", off)
#endif
#include <signal.h>
#include <stdio.h>
// enable the test framework
#define VIVA_TEST_FRAMEWORK_ENABLED 1
#ifdef VIVA_DEBUG_ENABLED
#undef VIVA_DEBUG_ENABLED
#endif
#define VIVA_DEBUG_ENABLED 1
#include "internal/viva_internal_export.h"
#define VIVA_UNUSED(x)
#define VIVA_TEST_DEFAULT_CTOR_COUNTER 1000
#define ADD_TOTAL() (total++)
#define ADD_FAILED() (failed++)
#ifdef _WIN32
#define VIVA_TEST_REGISTER_SIGNAL_HANDLER()                                                                            \
	signal(SIGSEGV, signal_handler);                                                                                     \
	signal(SIGABRT, signal_handler);
#elifdef __linux__
#define VIVA_TEST_REGISTER_SIGNAL_HANDLER()                                                                            \
	signal(SIGSEGV, signal_handler);                                                                                     \
	signal(SIGABRT, signal_handler);                                                                                     \
	signal(SIGTRAP, signal_handler);
#endif
#define VIVA_TEST_SETUP_FUNC(_expr)                                                                                                   \
	static int											 failed = 0;                                                                                        \
	static int											 total	= 0;                                                                                        \
	__attribute__((destructor)) void finish();                                                                                          \
	static void											 signal_handler(int sig) {                                                                          \
		 fprintf(stderr, "Signal %d received in line %d, file %s. Aborting...\n", sig, __LINE__, __FILE__, __func__); \
		 fflush(stderr);                                                                                              \
		 failed++;                                                                                                    \
		 finish();                                                                                                    \
	}                                                                                                                                   \
	__attribute__((constructor(VIVA_TEST_DEFAULT_CTOR_COUNTER))) void setup() {                                                         \
		VIVA_TEST_REGISTER_SIGNAL_HANDLER();                                                                                              \
		(void)_expr;                                                                                                                      \
		fprintf(stdout, "Running tests...\n");                                                                                            \
		fflush(stdout);                                                                                                                   \
	}

#define VIVA_TEST_SETUP_COMMON_FUNC()                                                                                  \
	__attribute__((destructor)) void finish() {                                                                          \
		FILE *out_stream = failed ? stderr : stdout;                                                                       \
		fprintf(out_stream, "\n");                                                                                         \
		fprintf(out_stream, "Tests finished.\n");                                                                          \
		fprintf(out_stream, "Total: %d, Passed: %d, Failed: %d\n", total, total - failed, failed);                         \
		if (failed) {                                                                                                      \
			fprintf(out_stream, "Test failed.\n");                                                                           \
		} else                                                                                                             \
			fprintf(out_stream, "Test passed.\n");                                                                           \
		fflush(out_stream);                                                                                                \
	}                                                                                                                    \
	int main(int, char **) {} /* nothing */

#define VIVA_TEST_SETUP_0(_expr) /* no args */                                                                         \
	VIVA_TEST_SETUP_FUNC(0)                                                                                              \
	VIVA_TEST_SETUP_COMMON_FUNC()

#define VIVA_TEST_SETUP_1(_expr)                                                                                       \
	VIVA_TEST_SETUP_FUNC(_expr)                                                                                          \
	VIVA_TEST_SETUP_COMMON_FUNC()

#define VIVA_TEST_UNIQUE_FUNC(counter)                                                                                 \
	__attribute__((constructor(counter + VIVA_TEST_DEFAULT_CTOR_COUNTER))) void EXPAND_COUNTER(test, counter)()

#define VIVA_TEST_IMPL_0() VIVA_TEST_IMPL_1(unnamed)

#define VIVA_TEST_IMPL_2(name, sub_name) VIVA_TEST_IMPL_1(name##_##sub_name)

#define VIVA_TEST_IMPL_1(name)                                                                                         \
	VIVA_TEST_UNIQUE_FUNC(VIVA_COUNTER) {                                                                                \
		fprintf(stdout, "Running test \'%s\'\n", #name);                                                                   \
		fflush(stdout);                                                                                                    \
		ADD_TOTAL();                                                                                                       \
	}                                                                                                                    \
	VIVA_TEST_UNIQUE_FUNC(VIVA_COUNTER)

#define VIVA_TEST(...) VIVA__VFUNC(VIVA_TEST_IMPL, ##__VA_ARGS__)
#define VIVA_TEST_SETUP(...) VIVA__VFUNC(VIVA_TEST_SETUP, ##__VA_ARGS__)
#define VIVA_EXPECT_EQ(x, y) VIVA_RUNTIME_REQUIRE(x, y)
#define VIVA_EXPECT_TRUE(x) VIVA_RUNTIME_REQUIRE(x)

#ifndef VIVA_EXPORT_OFF
#define EXPECT_EQ(x, y) VIVA_EXPECT_EQ(x, y)
#define EXPECT_TRUE(x) VIVA_EXPECT_TRUE(x)
#define EXPECT_NULL(x) VIVA_EXPECT_EQ(x, nullptr)
#define TEST(...) VIVA_TEST(__VA_ARGS__)
#define TEST_SETUP(...) VIVA_TEST_SETUP(__VA_ARGS__)
#endif
