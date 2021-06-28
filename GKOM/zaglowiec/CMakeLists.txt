cmake_minimum_required(VERSION 3.3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)

project(zaglowiec)

if (UNIX)
    if (POLICY CMP0072)
        cmake_policy(SET CMP0072 NEW)
    endif ()
    set(FREETYPE_INCLUDE_DIRS lib/common/freetype-2.10.0/include)
    set(FREETYPE_LIBRARY lib/common/freetype-2.10.0)
    find_package(OpenGL REQUIRED)
    find_package(Freetype REQUIRED)
    find_package(SFML 2.5 COMPONENTS audio REQUIRED)
    set(LIBS glfw GLEW)
    message(STATUS "Compiling linux")
elseif (WIN32)
    add_link_options(/NODEFAULTLIB:LIBCMT) # https://stackoverflow.com/questions/3007312/resolving-lnk4098-defaultlib-msvcrt-conflicts-with
    set(FREETYPE_INCLUDE_DIRS lib/common/freetype-2.10.0/include)
    set(FREETYPE_LIBRARY lib/common/freetype-2.10.0)
    find_package(Freetype REQUIRED)
    find_package(OpenGL REQUIRED)

    set(SFML_STATIC_LIBRARIES TRUE)
    set(SFML_DIR "lib/win/x64/SFML-2.5.1/lib/cmake/SFML")
    find_package(SFML 2.5.1 COMPONENTS audio REQUIRED)

    if (NOT CMAKE_CL_64)
        include_directories(lib/win/x86/include)

        if (${MSVC_TOOLSET_VERSION} EQUAL 100)      #= VS 2010 (10.0)
            link_directories(lib/win/x86/glfw/lib-vc2010)
        endif ()
        if (${MSVC_TOOLSET_VERSION} EQUAL 110)      #= VS 2012 (11.0)
            link_directories(lib/win/x86/glfw/lib-vc2012)
        endif ()
        if (${MSVC_TOOLSET_VERSION} EQUAL 120)      #= VS 2013 (12.0)
            link_directories(lib/win/x86/glfw/lib-vc2013)
        endif ()
        if (${MSVC_TOOLSET_VERSION} EQUAL 140)      #= VS 2015 (14.0)
            link_directories(lib/win/x86/glfw/lib-vc2015)
        endif ()
        if (${MSVC_TOOLSET_VERSION} EQUAL 141)      #= VS 2017 (15.0)
            link_directories(lib/win/x86/glfw/lib-vc2017)
        endif ()
        if (${MSVC_TOOLSET_VERSION} EQUAL 142)      #= VS 2019
            link_directories(lib/win/x86/glfw/lib-vc2019)
        endif ()

        link_directories(lib/win/x86)
        message(STATUS "Compiling x86")
    else ()
        include_directories(lib/win/x64/include)

        if (${MSVC_TOOLSET_VERSION} EQUAL 110)      #= VS 2012 (11.0)
            link_directories(lib/win/x64/glfw/lib-vc2012)
        endif ()
        if (${MSVC_TOOLSET_VERSION} EQUAL 120)      #= VS 2013 (12.0)
            link_directories(lib/win/x64/glfw/lib-vc2013)
        endif ()
        if (${MSVC_TOOLSET_VERSION} EQUAL 140)      #= VS 2015 (14.0)
            link_directories(lib/win/x64/glfw/lib-vc2015)
        endif ()
        if (${MSVC_TOOLSET_VERSION} EQUAL 141)      #= VS 2017 (15.0)
            link_directories(lib/win/x64/glfw/lib-vc2017)
        endif ()
        if (${MSVC_TOOLSET_VERSION} EQUAL 142)      #= VS 2019
            link_directories(lib/win/x64/glfw/lib-vc2019)
        endif ()

        link_directories(lib/win/x64)
        message(STATUS "Compiling x64")
    endif ()
    set(LIBS glfw3 glew32s sfml-audio)
endif ()

include_directories(project/include)
include_directories(lib/common/include)

add_library(zaglowiec-lib
        project/include/entity/complex/lighthouse.hpp
        project/include/entity/complex/sailing_ship.hpp
        project/include/entity/abstract_entity.hpp
        project/include/entity/entity.hpp
        project/include/entity/shape_factory.hpp
        project/src/entity/shape_factory.cpp
        project/src/lighting/light_factory.cpp
        project/include/model/model.hpp
        project/src/model/model.cpp
        project/include/model/shape.hpp
        project/src/model/shape.cpp
        project/include/model/textured_model.hpp
        project/include/renderer/entity_renderer.hpp
        project/src/renderer/entity_renderer.cpp
        project/include/renderer/master_renderer.hpp
        project/include/renderer/terrain_renderer.hpp
        project/include/renderer/skybox_renderer.hpp
        project/include/renderer/water_renderer.hpp
        project/src/renderer/skybox_renderer.cpp
        project/src/renderer/terrain_renderer.cpp
        project/src/renderer/water_renderer.cpp
        project/include/shader/entity_shader.hpp
        project/include/shader/shadowmap_shader.hpp
        project/include/shader/shader_program.hpp
        project/src/shader/shader_program.cpp
        project/include/shader/terrain_shader.hpp
        project/include/shader/water_shader.hpp
        project/include/texture/texture.hpp
        project/src/texture/texture.cpp
        project/include/texture/texture_manager.hpp
        project/src/texture/texture_manager.cpp
        project/include/camera.hpp
        project/src/camera.cpp
        project/src/cubemap.cpp
        project/include/display_manager.hpp
        project/src/display_manager.cpp
        project/include/terrain.hpp
        project/src/terrain.cpp
        project/include/water.hpp
        project/src/water.cpp
        project/include/utils.hpp
        project/src/utils.cpp
        project/src/stb_image.cpp
        project/include/entity/complex/complex_entity.hpp project/src/entity/complex/sailing_ship.cpp project/include/entity/complex/torch.hpp project/src/entity/complex/lighthouse.cpp project/include/demo.hpp project/src/demo.cpp)

target_link_libraries(zaglowiec-lib OpenGL::GL ${LIBS})

add_executable(zaglowiec project/src/main.cpp)
target_link_libraries(zaglowiec zaglowiec-lib OpenGL::GL ${LIBS})

# copy /res folder to a folder containing zaglowiec binary
add_custom_command(TARGET zaglowiec POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:zaglowiec>/res
        )

# copy /res to /build (for running from Visual Studio)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${CMAKE_SOURCE_DIR}/build/res)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${CMAKE_SOURCE_DIR}/build/tests/res)
