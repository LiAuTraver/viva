//////////////////////////////////////////////////////////////////////////
/// @file viva.h
/// @brief A header file for C11, C17, and C23, providing
///					some useful macros and functions.
/// @author LiAuTrever
/// @copyright MIT 3.0
/// @note viva_export part
/// can be turned off by defining VIVA_EXPORT_OFF
/// if having conflicts with other libraries
/// or manually turn off some macros using `#undef`
//////////////////////////////////////////////////////////////////////////
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "internal/viva_internal_export.h"
#pragma region viva export macros
#ifndef VIVA_EXPORT_OFF
typedef struct VIVA_RESULT_T					result_t;
typedef enum VIVA_STATUS_ENUM					status_t;
#define VIVA_PRINTF_DEC_FORMAT(x) VIVA_PRINTF_DEC_FORMAT_IMPL(x)
#define print(x) VIVA_PRINT(x)
#define println(...) VIVA_PRINTLN(__VA_ARGS__)
#define fprint(x) VIVA_FPRINT(x)
#define fprintln(x) VIVA_FPRINTLN(x)
#define reflexpr(x) VIVA_REFLEXPR(x)
#define return_error(code, msg) VIVA_RETURN_ERROR(code, msg)
#define return_value(...) VIVA_RETURN_VALUE(__VA_ARGS__)
#define return_value_ptr(...) VIVA_RETURN_VALUE_PTR(__VA_ARGS__)
#define contract_assert(...) VIVA_RUNTIME_REQUIRE(__VA_ARGS__)
#define result_cast(...) VIVA_RESULT_CAST(__VA_ARGS__)
#define is_string(x) VIVA_IS_CHAR_PTR(x)
#define get_raw(...) VIVA_GET_FROM_STDIN_RAW(__VA_ARGS__)
#define get_rec(...) VIVA_GET_FROM_STDIN_RECURSIVE(__VA_ARGS__)
#define smart VIVA_SMART_PTR
#define alloc(...) VIVA_ALLOC(__VA_ARGS__)
#endif
#pragma endregion
#ifdef __cplusplus
}
#endif
