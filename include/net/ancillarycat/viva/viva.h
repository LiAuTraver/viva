//////////////////////////////////////////////////////////////////////////
/// @file viva.h
/// @brief A header file for C11, C17, and C23, providing
///					some useful macros and functions.
/// @author LiAuTrever
/// @copyright MIT 3.0
//////////////////////////////////////////////////////////////////////////
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "internal/predef.h"
#include "internal/fmt.h"
#include "internal/contract.h"
#include "internal/scan.h"
#include "internal/result_t.h"
/// @note any macro that contains `IMPL` or `VIVA__` is internal and shouldn't be used directly.
#pragma region viva export macros
#define VIVA_PRINT(x) VIVA_PRINT_IMPL(x)
#define VIVA_PRINTLN(x) VIVA_PRINTLN_IMPL(x)
#define VIVA_FPRINT(file_,x) VIVA_FPRINT_IMPL(file_,x)
#define VIVA_FPRINTLN(file_,x) VIVA_FPRINTLN_IMPL(file_,x)
#define VIVA_RETURN_ERROR(...) VIVA_RETURN_ERROR_IMPL(__VA_ARGS__);
#define VIVA_RETURN_VALUE(...) VIVA_RETURN_VALUE_IMPL(__VA_ARGS__);
#define VIVA_RETURN_VALUE_PTR(...) VIVA_RETURN_VALUE_PTR_IMPL(__VA_ARGS__);
#define VIVA_RUNTIME_REQUIRE(...) VIVA_RUNTIME_REQUIRE_IMPL(__VA_ARGS__)
#define VIVA_RESULT_CAST(...) VIVA_RESULT_CAST_IMPL(__VA_ARGS__)
#define VIVA_REFLEXPR(x) VIVA_REFLEXPR_IMPL(x)
#define VIVA_GET_ARGUMENT_TYPE_FOR_SCANF(...) VIVA_GET_ARGUMENT_TYPE_FOR_SCANF_IMPL(__VA_ARGS__)
#define VIVA_GET_FROM_STDIN_RAW(...) VIVA_GET_FROM_STDIN_RAW_IMPL(__VA_ARGS__)

#pragma endregion
//////////////////////////////////////////////////////////////////////////
/// viva_export part
/// can be turned off by defining VIVA_EXPORT_OFF
/// if having conflicts with other libraries
/// or manually turn off some macros using `#undef`
//////////////////////////////////////////////////////////////////////////
#pragma region viva export convinent macros
#ifndef VIVA_EXPORT_OFF
typedef struct VIVA_RESULT_T result_t;
typedef enum VIVA_STATUS_ENUM status_t;
#define VIVA_PRINTF_DEC_FORMAT(x) VIVA_PRINTF_DEC_FORMAT_IMPL(x)
#define print(x) VIVA_PRINT(x)
#define println(x) VIVA_PRINTLN(x)
#define fprint(x) VIVA_FPRINT(x)
#define fprintln(x) VIVA_FPRINTLN(x)
#define reflexpr(x) VIVA_REFLEXPR(x)
#define return_error(code, msg) VIVA_RETURN_ERROR(code, msg)
#define return_value(...) VIVA_RETURN_VALUE(__VA_ARGS__)
#define return_value_ptr(...) VIVA_RETURN_VALUE_PTR(__VA_ARGS__)
#define contract_assert(...) VIVA_RUNTIME_REQUIRE(__VA_ARGS__)
#define result_cast(...) VIVA_RESULT_CAST(__VA_ARGS__)
#define get_from_stdin_raw(...) VIVA_GET_FROM_STDIN_RAW(__VA_ARGS__)
#endif
#pragma endregion
#ifdef __cplusplus
}
#endif
