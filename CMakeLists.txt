cmake_minimum_required(VERSION 3.19)

project(memdb VERSION 0.1.0 LANGUAGES C)

# set c target to C99
set(CMAKE_C_STANDARD 99)

enable_testing()

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

# The following folder will be included
include_directories(PROJECT_SOURCE_DIR)
add_subdirectory(test)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/*.c)
file(GLOB HEADERS  ${PROJECT_SOURCE_DIR}/*.h) 

add_library(mendblib ${SOURCES} ${HEADERS})
add_executable(mendb ${PROJECT_SOURCE_DIR}/mendb.c)
target_link_libraries(mendb mendblib)