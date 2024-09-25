#include "include/ancillarycat/vtest.h"
#include <string.h>
#include "include/ancillarycat/list.h"

static inline result_t a_error_res() {
	return_error(kUnknownError, "Unknown error");
}

static inline result_t a_value_res() {
	var i = 1;
	return_value(i, 0);
}

static inline result_t a_value_ptr_res() {
	var i = "H";
	return_value_ptr(&i);
}

int failed = 0;
int passed = 0;
int total = 0;

__attribute__((constructor)) void setup() {
	fprintf(stdout, "Running tests for %s...\n", VIVA_STRINGIFY(VIVA_CONCAT(1.list)));
}

__attribute__((destructor)) void finish() {
	val out_stream = failed ? stderr : stdout;
	fprintf(out_stream, "\n");
	fprintf(out_stream, "Tests finished.\n");
	fprintf(out_stream, "Total: %d, Passed: %d, Failed: %d\n", total, passed, failed);
	if (failed) { fprintf(out_stream, "Test %s failed.\n", VIVA_STRINGIFY(VIVA_CONCAT(1.list))); } else fprintf(
		out_stream, "Test %s passed.\n", VIVA_STRINGIFY(VIVA_CONCAT(1.list)));
	fflush(out_stream);
}

int main(int, char **) {
	auto lh = List.empty();
	List.push_back(lh, 1);
	List.push_back(lh, 2);
	List.push_back(lh, 3);
	List.push_back(lh, 4);
	List.push_back(lh, 5);
	List.push_back(lh, 6);
	List.reverse(lh);
}

// TEST(123,buy){
// 	VIVA_STRINGIFY(test);
// 	val res1 = a_error_res();
// 	EXPECT_EQ(res1.error.code, kERR_UNKNOWN);
// 	EXPECT_EQ(res1.has_value, false);
//
// 	val res2 = a_value_res();
// 	EXPECT_EQ(result_cast(res2,any.int_type), 1);
// 	EXPECT_EQ(res2.has_value, true);
//
// 	val res3 = a_value_ptr_res();
// 	EXPECT_EQ(res3.has_value, true);
// }
