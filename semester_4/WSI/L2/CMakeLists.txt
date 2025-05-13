cmake_minimum_required(VERSION 3.10)
project(NPuzzle)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(src/include)

# Source files
set(SOURCES
    src/cpp/NPuzzle.cpp
    src/main/main.cpp
)

# Create executable
add_executable(NPuzzle ${SOURCES})

# If you have any libraries to link against, add them here
# target_link_libraries(NPuzzle PRIVATE some_library)