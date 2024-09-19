#if _MSC_VER
#define CRT_SECURE_NO_WARNINGS 1
#endif
#ifndef _DEBUG
#define _DEBUG 1
#endif
#include <stdio.h>
#include "test.h"
#include "viva.h"
result_t a_error_res(){
	return_error(kERR_UNKNOWN, "Unknown error");
}
int i = 1;
int* i_ptr = &i;
result_t a_value_res(){
	return_value(i_ptr, "int*", 0);
}
int main(void) {
	print("Hello, World!");
	test_typename();
	val res = a_error_res();
	if(res.has_value){
		printf("Value: %p\n", res.value.ptr);
	}else{
		printf("Error: %s\n", res.error.message);
	}
	val res2 = a_value_res();
	if(res2.has_value && res2.value.type_t == "int*"){
		printf("Value: %p\n", &res2.value.ptr);
	}else{
		printf("Error: %s\n", res2.error.message);
	}
	contract_assert(i,1);
	return 0;
}
