cmake_minimum_required(VERSION 3.10)

project(
  wengine
  VERSION 1.0.0
  DESCRIPTION "engine in SDL")

set(GLEW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/GLEW)

set(HOST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/host/main_host.c
                 ${CMAKE_CURRENT_SOURCE_DIR}/src/logger.c)

set(MODULE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/logger.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/g_defs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/player.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/render.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/update.c)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# compile SDL
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2)
# compile SDL_image
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL_image)
# compile GLEW
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/GLEW/build/cmake)

find_package(OpenGL REQUIRED)

# global include
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# module
add_library(${PROJECT_NAME}-module SHARED ${MODULE_SOURCES})
set_target_properties(
  ${PROJECT_NAME}-module PROPERTIES VERSION ${PROJECT_VERSION}
                                    DESCRIPTION ${PROJECT_DESCRIPTION})

target_link_libraries(${PROJECT_NAME}-module PRIVATE SDL2 SDL2_image glew)

# host
add_executable(${PROJECT_NAME} ${HOST_SOURCES})
set_target_properties(
  ${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} DESCRIPTION
                                                        ${PROJECT_DESCRIPTION})

add_compile_options(-Wall -Wextra)
