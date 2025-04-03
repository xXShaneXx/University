cmake_minimum_required(VERSION 3.10)
project(EX1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=performance-*,readability-*,modernize-*,clang-analyzer-*")

# Set the output directory for all executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/src/include)

# Add subdirectories
add_subdirectory(src/generators)
add_subdirectory(src/sort)

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)

# Ensure Google Test is built
FetchContent_MakeAvailable(googletest)

enable_testing()
add_subdirectory(tests)