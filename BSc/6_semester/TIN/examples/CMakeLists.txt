cmake_minimum_required(VERSION 3.16)
project(examples)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(../lib/loguru)
link_directories(../lib/loguru)

add_executable(logging logging.cpp)

target_link_libraries(logging loguru pthread dl)
