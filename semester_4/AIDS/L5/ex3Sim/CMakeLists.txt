cmake_minimum_required(VERSION 3.10)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# Add source files
file(GLOB SOURCES1 ${CMAKE_SOURCE_DIR}/src/cpp/*.cpp ${CMAKE_SOURCE_DIR}/src/main/main.cpp)
file(GLOB SOURCES2 ${CMAKE_SOURCE_DIR}/src/cpp/*.cpp ${CMAKE_SOURCE_DIR}/src/main/main2.cpp)

# run git clone https://github.com/alandefreitas/matplotplusplus/ in CMakeList.txt folder
add_subdirectory(matplotplusplus)


# Add the executable
add_executable(experiment1 ${SOURCES1})
add_executable(experiment2 ${SOURCES2})

target_link_libraries(experiment1 PRIVATE matplot)
target_link_libraries(experiment2 PRIVATE matplot)