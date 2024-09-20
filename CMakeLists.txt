cmake_minimum_required(VERSION 3.30)
project(viva C)

set(CMAKE_C_STANDARD 23)
add_executable(viva main.c
	 include/viva.h
	 include/internal/fmt.h
	 include/internal/predef.h
	 include/internal/result_t.h
	 include/internal/contract.h
)
add_executable(viva_test test.c
	 include/test.h
	 include/viva.h
	 include/internal/fmt.h
	 include/internal/predef.h
	 include/internal/result_t.h
	 include/internal/contract.h
)
