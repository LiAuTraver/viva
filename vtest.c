#include "include/vtest.h"

#include <string.h>

static inline result_t a_error_res() {
	return_error(kERR_UNKNOWN, "Unknown error");
}

static inline result_t a_value_res() {
	var i = 1;
	return_value(i, 0);
}

static inline result_t a_value_ptr_res() {
	var i = "H";
	return_value_ptr(&i);
}

TEST_SETUP()

	val res1 = a_error_res();
	EXPECT_EQ(res1.error.code, kERR_UNKNOWN);
	EXPECT_EQ(res1.has_value, false);

	val res2 = a_value_res();
	EXPECT_EQ(result_cast(res2,any.int_type), 1);
	EXPECT_EQ(res2.has_value, true);

	val res3 = a_value_ptr_res();
	EXPECT_EQ(res3.has_value, true);

TEST_FINISH()
