cmake_minimum_required(VERSION 3.10)

# Set the project name
project(AI_Search)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=performance-*,readability-*,modernize-*,clang-analyzer-*")

# Set the output directory for all executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/src/include)

# Find all source files in src/cpp and src/main
file(GLOB SOURCES
    "src/cpp/*.cpp"
    "src/main/*.cpp"
)

# Add executable with all source files
add_executable(${PROJECT_NAME}
    ${SOURCES}
)

# Add any additional libraries if needed
# target_link_libraries(${PROJECT_NAME} PRIVATE library_name)