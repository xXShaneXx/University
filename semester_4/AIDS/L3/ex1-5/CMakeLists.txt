cmake_minimum_required(VERSION 3.10)

project(select LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Less aggressive warnings for now
add_compile_options(-Wall -Wextra -pedantic)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/src/include)

# Add subdirectories
add_subdirectory(simulationframework)

# Matplot++, 
# run git clone https://github.com/alandefreitas/matplotplusplus/ in CMakeList.txt folder
add_subdirectory(matplotplusplus)

# Gather all .cpp files in the src/main folder
file(GLOB SIM_SOURCES ${PROJECT_SOURCE_DIR}/src/simulations/*.cpp)

# Add an executable for each .cpp file and link libraries
foreach(SOURCE_FILE ${SIM_SOURCES})
    get_filename_component(EXECUTABLE_NAME ${SOURCE_FILE} NAME_WE)
    add_executable(${EXECUTABLE_NAME} ${SOURCE_FILE})
    
    # Link required libraries
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE matplot simframework::SimulationFramework)
endforeach()

# Add executable
add_executable(generator src/cpp/random-generator.cpp)
add_executable(select src/cpp/select.cpp)
add_executable(random-select src/cpp/random-select.cpp)

# Set output directory for plots
set(OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/plots/)

# Enable testing
enable_testing()

# Add Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add test executable
add_executable(test_QuickSorts src/tests/test_QuickSorts.cpp)
target_link_libraries(test_QuickSorts PRIVATE GTest::GTest GTest::Main)

# Add test to CTest
add_test(NAME QuickSortsTests COMMAND test_QuickSorts)

