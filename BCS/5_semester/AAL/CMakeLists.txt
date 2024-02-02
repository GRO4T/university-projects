cmake_minimum_required(VERSION 3.5)

project(RobotGUI LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(AUTOGEN_BUILD_DIR inc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif ()

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

find_package(Boost COMPONENTS program_options REQUIRED)

include_directories(inc)
include_directories(${Boost_INCLUDE_DIR})

add_library(cmyk_sorter STATIC
        inc/algorithm/generator.hpp
        src/algorithm/generator.cc
        inc/algorithm/alg_params.hpp

        inc/algorithm/graph.hpp
        src/algorithm/graph.cc

        inc/algorithm/robot.hpp
        src/algorithm/robot.cc

        inc/algorithm/naive.hpp
        src/algorithm/naive.cc

        inc/algorithm/bfs.hpp
        src/algorithm/bfs.cc

        inc/algorithm/astar.hpp
        src/algorithm/astar.cc

        inc/algorithm/cmyk_algorithm.hpp
        inc/algorithm/algorithm.hpp
        src/algorithm/cmyk_algorithm.cc

        inc/algorithm/property.hpp
        inc/algorithm/benchmark.hpp)

add_executable(RobotGUI
    src/main.cpp
        src/gui/mainwindow.cpp
        inc/gui/mainwindow.h
        src/gui/mainwindow.ui
        inc/algorithm/alg_params.hpp

        src/gui/inputstringdialog.ui
        inc/gui/inputstringdialog.h
        src/gui/inputstringdialog.cpp

        inc/gui/container.h
        src/gui/container.cpp

        inc/gui/robot.h
        src/gui/robot.cpp

        inc/gui/aalbackend.h
        src/gui/aalbackend.cpp

        inc/gui/inputstringvalidator.h
        src/gui/inputstringvalidator.cpp
)

add_executable(robot-cli src/main-cli.cpp)
add_executable(data-generator src/data-generator.cpp)

target_link_libraries(RobotGUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets cmyk_sorter pthread)
target_link_libraries(robot-cli cmyk_sorter pthread ${Boost_LIBRARIES})
target_link_libraries(data-generator cmyk_sorter pthread ${Boost_LIBRARIES})

add_subdirectory(tests)

add_custom_target(CopyCompileCommands ALL
    ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)

set(TOOLS
    benchmark.py
    benchmark2.py
    plot.py
    plot2.py
    table.py
    table2.py
)

foreach (file_i ${TOOLS})
    add_custom_command(
        TARGET robot-cli
        POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy ${CMAKE_SOURCE_DIR}/tools/${file_i} ${CMAKE_BINARY_DIR}/
    )

endforeach ()

