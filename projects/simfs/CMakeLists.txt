cmake_minimum_required(VERSION 3.10)

project("simfs" VERSION 1.1.0 LANGUAGES C)

if(NOT DEFINED CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

file(GLOB_RECURSE SRC_FILES "*.c" "*.h")
add_executable(${PROJECT_NAME}
  ${SRC_FILES}
)

set_target_properties(simfs PROPERTIES LINKER_LANGUAGE C)