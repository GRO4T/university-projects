cmake_minimum_required(VERSION 3.5)

project(Qt-sensors-widgets VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        sensor_model/sensor.h
        sensor_model/device.h
        sensor_model/sensor_model.cpp
        sensor_model/sensor_model.h
        sensor_model/sensor_probe.h
        sensor_model/command_executor.h
        widgets/sensor_text_value_widget.h
        process_list_model/process_list_model.h
        process_list_model/process_list_model.cpp
        process_list_model/process_list_delegate.h
        )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Qt-sensors-widgets
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Qt-sensors-widgets APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Qt-sensors-widgets SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Qt-sensors-widgets
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Qt-sensors-widgets PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(Qt-sensors-widgets PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Qt-sensors-widgets)
endif()
