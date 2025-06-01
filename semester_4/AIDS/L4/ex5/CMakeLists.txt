cmake_minimum_required(VERSION 3.10)

# Set the project name
project(AIDS_L4)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# Add source files
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/cpp/*.cpp ${CMAKE_SOURCE_DIR}/src/main/*.cpp)

# Add the executable
add_executable(ex5 ${SOURCES})