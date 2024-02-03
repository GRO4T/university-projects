cmake_minimum_required(VERSION 3.16)
project(TinyAd)
set(CMAKE_CXX_STANDARD 17)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g -fprofile-arcs -ftest-coverage -fPIC -O0")
# set(CMAKE_CXX_FLAGS_DEBUG "-g -fPIC -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -static-libstdc++")

add_subdirectory(lib/googletest)
include_directories(lib/googletest include)

add_subdirectory(lib/fmt)
include_directories(lib/fmt/include)

add_subdirectory(lib/loguru)
include_directories(lib/loguru)

add_subdirectory(lib/stduuid)
include_directories(lib/stduuid/include)

add_subdirectory(lib/CRCpp)
include_directories(lib/CRCpp)

file (GLOB tiny_ad_sources
    "include/*.hpp"
    "src/*.cpp"
)

file (GLOB tiny_ad_test_unit_sources
    "test/unit/*.cpp"
)

file (GLOB tiny_ad_test_apps_sources
    "test/apps/*.cpp"
)

add_library(project-lib ${tiny_ad_sources})
target_link_libraries(project-lib fmt loguru pthread dl stduuid sqlite3 CRCpp)

add_executable(server src/server/main.cpp)
add_executable(client src/client/main.cpp)

add_executable(schedule_parsing test/apps/schedule_parsing.cpp)
add_executable(database_client test/apps/database_client.cpp)
add_executable(test ${tiny_ad_test_unit_sources})

target_link_libraries(schedule_parsing project-lib)
target_link_libraries(server project-lib)
target_link_libraries(client project-lib)
target_link_libraries(database_client project-lib)
target_link_libraries(test project-lib gtest gtest_main)

add_custom_target(
    clang-format
    COMMAND clang-format -i ${tiny_ad_sources} ${tiny_ad_test_unit_sources} ${tiny_ad_test_apps_sources}
)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_MODULE_PATH ./CMakeModules)
    if(CMAKE_COMPILER_IS_GNUCXX)
        include(CodeCoverage)
        setup_target_for_coverage_gcovr_html(
            NAME coverage
            EXCLUDE lib
            EXECUTABLE ./bin/test)
    endif()
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)
