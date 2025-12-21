cmake_minimum_required(VERSION 3.10)
project(ex2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(ex2 main.cpp ../reader/reader.cpp)

target_include_directories(ex2 PRIVATE ../reader)
target_compile_definitions(ex2 PRIVATE TEST_DIR=${CMAKE_CURRENT_SOURCE_DIR}/../tests)
