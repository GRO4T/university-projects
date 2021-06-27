cmake_minimum_required(VERSION 3.10)
project(ZombieShooter)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(ZombieShooter src/main.cc src/game.cc src/asset_manager.cc src/camera.cc src/world.cc src/entity/entity.cc include/entity/player.h include/entity/movement_controller.h src/utils.cc include/entity/animation/animation_controller.h src/entity/player.cc include/entity/animation/player_animation_controller.h include/scene.h src/scene.cc include/entity/animation/animation_clip.h include/debug_manager.h src/debug_manager.cc include/entity/behaviour.h src/background_manager.cc include/background_manager.h)

# create symlink to /resources folder
set(source "${CMAKE_SOURCE_DIR}/resources")
set(destination "${CMAKE_CURRENT_BINARY_DIR}/resources")
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
        DEPENDS ${destination}
        COMMENT "symbolic link resources folder from ${source} => ${destination}"
)

if (UNIX)
    add_subdirectory(lib/tmxparser/tmxparser-master)
    target_link_libraries(ZombieShooter Box2D sfml-graphics sfml-audio sfml-window sfml-system tmxparser)
endif (UNIX)

# Notice no tmxparser library. Since using it I've switched to Linux and haven't checked whether it is installable on
# Windows
if (WIN32)
    # SFML
    set(SFML_STATIC_LIBRARIES TRUE)
    set(SFML_DIR "lib/SFML-2.5.1/lib/cmake/SFML")
    find_package(SFML 2.5.1 COMPONENTS graphics window system audio REQUIRED)
    # Box2D
    add_subdirectory("lib/box2d")
    target_link_libraries(ZombieShooter box2d sfml-graphics sfml-audio sfml-window sfml-system)
endif (WIN32)
