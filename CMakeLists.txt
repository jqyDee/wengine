cmake_minimum_required(VERSION 3.10)

project(sdl2-hotreload)
set(HOST_SOURCES "src/main_host.c")
set(MODULE_SOURCES "src/main_module.c")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CUTILS_LIB_DIR "/usr/local/lib/libc-utils.dylib")
set(CUTILS_INC_DIR "/usr/local/include/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS}
                    ${CUTILS_INC_DIR} include)

add_library(${PROJECT_NAME}-module SHARED ${MODULE_SOURCES})

target_link_libraries(
  ${PROJECT_NAME}-module PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES}
                                 ${CUTILS_LIB_DIR})

add_executable(${PROJECT_NAME}-host ${HOST_SOURCES})

target_link_libraries(${PROJECT_NAME}-host PRIVATE ${CUTILS_LIB_DIR})

add_compile_options(-Wall -Wextra)
