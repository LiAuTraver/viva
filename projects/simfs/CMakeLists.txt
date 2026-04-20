cmake_minimum_required(VERSION 3.20)

project("simfs" VERSION 1.1.0 LANGUAGES C)

if(NOT DEFINED CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 11)
endif()

set(VIVA_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}../../include)
file(GLOB_RECURSE VIVA_HEADERS ${VIVA_SOURCE}/*.h)
include_directories(${VIVA_SOURCE})

file(GLOB_RECURSE SRC_FILES "*.c" "*.h")
add_executable(${PROJECT_NAME}
  ${SRC_FILES}
)

set_target_properties(stumgr PROPERTIES LINKER_LANGUAGE C)