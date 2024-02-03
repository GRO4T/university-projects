cmake_minimum_required(VERSION 3.10)

project(Ketje VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add GoogleTest
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

# Add KeccakTools
add_subdirectory(lib/KeccakTools)

# Build tests
enable_testing()

# Test KeccakTools implementation
add_executable(KeccakToolsTest test/test_ketje_keccaktools.cpp)
target_include_directories(KeccakToolsTest PRIVATE lib/KeccakTools)
target_link_libraries(KeccakToolsTest PRIVATE GTest::GTest KeccakTools)
add_test(UnitTests KeccakToolsTest)

# Test our Ketje implementation
add_executable(KetjeTest test/test_ketje.cpp src/monkeywrap.cpp src/monkeyduplex.cpp src/utils.cpp)
target_include_directories(KetjeTest PRIVATE include lib/KeccakTools)
target_link_libraries(KetjeTest PRIVATE GTest::GTest KeccakTools)
add_test(UnitTests KetjeTest)
target_compile_definitions(KetjeTest PRIVATE -DKETJE_JR)

# Build source
add_executable(KetjeJr main.cpp src/monkeywrap.cpp src/monkeyduplex.cpp src/utils.cpp)
target_include_directories(KetjeJr PRIVATE include lib/KeccakTools)
target_link_libraries(KetjeJr PRIVATE KeccakTools)
target_compile_definitions(KetjeJr PRIVATE -DKETJE_JR)

add_executable(KetjeSr main.cpp src/monkeywrap.cpp src/monkeyduplex.cpp src/utils.cpp)
target_include_directories(KetjeSr PRIVATE include lib/KeccakTools)
target_link_libraries(KetjeSr PRIVATE KeccakTools)
target_compile_definitions(KetjeSr PRIVATE -DKETJE_SR)

add_executable(KetjeMinor main.cpp src/monkeywrap.cpp src/monkeyduplex.cpp src/utils.cpp)
target_include_directories(KetjeMinor PRIVATE include lib/KeccakTools)
target_link_libraries(KetjeMinor PRIVATE KeccakTools)
target_compile_definitions(KetjeMinor PRIVATE -DKETJE_MINOR)

add_executable(KetjeMajor main.cpp src/monkeywrap.cpp src/monkeyduplex.cpp src/utils.cpp)
target_include_directories(KetjeMajor PRIVATE include lib/KeccakTools)
target_link_libraries(KetjeMajor PRIVATE KeccakTools)
target_compile_definitions(KetjeMajor PRIVATE -DKETJE_MAJOR)
