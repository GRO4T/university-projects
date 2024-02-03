cmake_minimum_required(VERSION 3.16.3)

project(PORR)

set(CMAKE_CXX_STANDARD 11)

####################################
# Main executable
####################################

add_executable(runner 
    src/programs/runner.cpp
    src/plots.cpp
    src/random_search.cpp
    src/test_functions.cpp
    src/utilities.cpp
    src/swarm_search.cpp
)

target_compile_options(runner PRIVATE -fopenmp -Wall)

target_link_options(runner PRIVATE -fopenmp)

target_include_directories(runner PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    /usr/include/python3.10
)

target_link_libraries(runner PRIVATE python3.10)

####################################
# Plots
####################################

add_executable(plotter
    src/programs/plotter.cpp
    src/test_functions.cpp
    src/plots.cpp
    src/random_search.cpp
    src/swarm_search.cpp
)
target_compile_options(plotter PRIVATE -fopenmp -Wall)

target_link_options(plotter PRIVATE -fopenmp)
target_link_libraries(plotter PRIVATE python3.10)
target_include_directories(plotter PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    /usr/include/python3.10
)
