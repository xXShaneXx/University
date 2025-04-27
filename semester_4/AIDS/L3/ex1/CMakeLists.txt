cmake_minimum_required(VERSION 3.10)

project(select LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Less aggressive warnings for now
add_compile_options(-Wall -Wextra -pedantic)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/src/include)

# Add executable
add_executable(select src/main/select.cpp)
add_executable(select-random src/main/random-select.cpp)


# Gather all .cpp files in the src/main folder
##file(GLOB MAIN_SOURCES ${PROJECT_SOURCE_DIR}/src/main/*.cpp)

# Add an executable for each .cpp file
##foreach(SOURCE_FILE ${MAIN_SOURCES})
##    get_filename_component(EXECUTABLE_NAME ${SOURCE_FILE} NAME_WE)
##    add_executable(${EXECUTABLE_NAME} ${SOURCE_FILE})
##endforeach()
