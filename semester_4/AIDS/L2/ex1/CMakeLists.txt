cmake_minimum_required(VERSION 3.10)
project(EX1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=performance-*,readability-*,modernize-*,clang-analyzer-*")

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/src/include)

# Add subdirectories
add_subdirectory(src/cpp)
add_subdirectory(src/generators)
add_subdirectory(src/sort)

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)

enable_testing()
add_subdirectory(tests)