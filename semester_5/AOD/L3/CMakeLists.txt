project(ShortestPaths)

cmake_minimum_required(VERSION 4.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")

include_directories(include)

set(SOURCES 
    main.cpp 
    src/graph.cpp 
    src/dijkstra.cpp 
    src/dial.cpp
    src/radixHeap.cpp
)

# 1. Dijkstra Standardowy
add_executable(dijkstra ${SOURCES})
target_compile_definitions(dijkstra PRIVATE ALGO_NAME="dijkstra" ALGO_TYPE=0)

# 2. Algorytm Diala
add_executable(dial ${SOURCES})
target_compile_definitions(dial PRIVATE ALGO_NAME="dial" ALGO_TYPE=1)

# 3. Radix Heap
add_executable(radixheap ${SOURCES})
target_compile_definitions(radixheap PRIVATE ALGO_NAME="radixheap" ALGO_TYPE=2)