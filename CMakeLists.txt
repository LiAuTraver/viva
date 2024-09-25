cmake_minimum_required(VERSION 3.30)
project(viva C)

set(CMAKE_C_STANDARD 23)
add_executable(viva main.c
)
add_executable(viva_test vtest.c
)
