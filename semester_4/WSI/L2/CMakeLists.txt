cmake_minimum_required(VERSION 3.10)
project(NPuzzle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(src/include)

# Matplot++, 
# run git clone https://github.com/alandefreitas/matplotplusplus/ in CMakeList.txt folder
add_subdirectory(matplotplusplus)

# Common sources
set(NPUZZLE_SOURCES
    src/cpp/NPuzzle.cpp
    src/cpp/DisjointPatternDB.cpp
)

# Executables for each main
add_executable(NPuzzle ${NPUZZLE_SOURCES} src/main/main.cpp)
add_executable(Load8 ${NPUZZLE_SOURCES} src/main/load8.cpp)
add_executable(Save8 ${NPUZZLE_SOURCES} src/main/save8.cpp)
add_executable(Load15 ${NPUZZLE_SOURCES} src/main/load15.cpp)
add_executable(Save15 ${NPUZZLE_SOURCES} src/main/save15.cpp)
add_executable(Simulation ${NPUZZLE_SOURCES} src/main/simulation.cpp)
target_link_libraries(Simulation matplot)