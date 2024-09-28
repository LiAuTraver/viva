cmake_minimum_required(VERSION 3.30)
project(viva C)

set(VIVA_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/include/net/ancillarycat/viva)
# add all header files
file(GLOB_RECURSE VIVA_HEADERS ${VIVA_SOURCE}/*.h)
add_executable(viva
	 main.c
	 ${VIVA_HEADERS}
)
add_executable(viva_test
	 vtest.c
	 ${VIVA_HEADERS}
)
