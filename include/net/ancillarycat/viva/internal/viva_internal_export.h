#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "contract.h"
#include "fmt.h"
#include "predef.h"
#include "result_t.h"
#include "scan.h"
#include "memory.h"
//! @note any macro that contains `IMPL` or `VIVA__` is internal and shouldn't be used directly.
#pragma region viva export macros
#define VIVA_PRINT(x) VIVA_PRINT_IMPL(x)
#define VIVA_PRINTLN(x) VIVA_PRINTLN_IMPL(x)
#define VIVA_FPRINT(file_, x) VIVA_FPRINT_IMPL(file_, x)
#define VIVA_FPRINTLN(file_, x) VIVA_FPRINTLN_IMPL(file_, x)
#define VIVA_RETURN_ERROR(...) VIVA_RETURN_ERROR_IMPL(__VA_ARGS__);
#define VIVA_RETURN_VALUE(...) VIVA_RETURN_VALUE_IMPL(__VA_ARGS__);
#define VIVA_RETURN_VALUE_PTR(...) VIVA_RETURN_VALUE_PTR_IMPL(__VA_ARGS__);
#define VIVA_RUNTIME_REQUIRE(...) VIVA_RUNTIME_REQUIRE_IMPL(__VA_ARGS__)
#define VIVA_RESULT_CAST(...) VIVA_RESULT_CAST_IMPL(__VA_ARGS__)
#define VIVA_REFLEXPR(x) VIVA_REFLEXPR_IMPL(x)
#define VIVA_GET_ARGUMENT_TYPE_FOR_SCANF(...) VIVA_GET_ARGUMENT_TYPE_FOR_SCANF_IMPL(__VA_ARGS__)
#define VIVA_GET_FROM_STDIN_RAW(...) VIVA_GET_FROM_STDIN_RAW_IMPL(__VA_ARGS__)
#define VIVA_IS_CHAR_PTR(x) VIVA_IS_CHAR_PTR_IMPL(x)
#define VIVA_GET_FROM_STDIN_RECURSIVE(...) VIVA_GET_FROM_STDIN_RECURSIVE_IMPL(__VA_ARGS__)
#define VIVA_SMART_PTR VIVA_AUTO_PTR_
#pragma endregion
typedef struct VIVA_RESULT_T	viva_result_t;
typedef enum VIVA_STATUS_ENUM viva_status_t;
#ifdef __cplusplus
}
#endif
