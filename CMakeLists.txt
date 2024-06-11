cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

project(
  wengine
  VERSION 1.0.0
  DESCRIPTION "engine in SDL")

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

set(SDL_DIR ${LIB_DIR}/SDL2)
set(SDL_IMAGE_DIR ${LIB_DIR}/SDL2_image)

find_package(Vulkan REQUIRED)
# compile SDL
add_subdirectory(${SDL_DIR})
# compile SDL_image
add_subdirectory(${SDL_IMAGE_DIR})

# global include
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src ${Vulkan_INCLUDE_DIRS})

# module
set(MODULE_SOURCES
    ${SOURCE_DIR}/game.c
    ${SOURCE_DIR}/logger.c
    ${SOURCE_DIR}/g_defs.c
    ${SOURCE_DIR}/player.c
    ${SOURCE_DIR}/render.c
    ${SOURCE_DIR}/update.c
    ${SOURCE_DIR}/g_sdl.c)

add_library(${PROJECT_NAME}-module SHARED ${MODULE_SOURCES})
set_target_properties(
  ${PROJECT_NAME}-module PROPERTIES VERSION ${PROJECT_VERSION}
                                    DESCRIPTION ${PROJECT_DESCRIPTION})

target_link_libraries(${PROJECT_NAME}-module PRIVATE SDL2 SDL2_image
                                                     ${Vulkan_LIBRARIES})

# host
set(HOST_SOURCES ${SOURCE_DIR}/host/main_host.c ${SOURCE_DIR}/logger.c)

add_executable(${PROJECT_NAME} ${HOST_SOURCES})
set_target_properties(
  ${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} DESCRIPTION
                                                        ${PROJECT_DESCRIPTION})

add_compile_options(-Wall -Wextra)
add_link_options(-Wall -Wextra)
