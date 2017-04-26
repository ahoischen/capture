cmake_minimum_required(VERSION 3.7.0)

project(capture C CXX)

set(CMAKE_CXX_STANDART 11)
set(CMAKE_CXX_STANDART_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(Tools3DS)
find_package(CTRULIB REQUIRED)

set(CAPTURE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/source)
set(CAPTURE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(CAPTURE_TEST_DIR ${PROJECT_SOURCE_DIR}/tests)
set(CATCH_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/Catch/single_include)

file(GLOB CAPTURE_SOURCE_FILES ${CAPTURE_SOURCE_DIR}/*.cpp)
include_directories(${CAPTURE_INCLUDE_DIR} ${CATCH_INCLUDE_DIR})

add_definitions(
    -DCATCH_CONFIG_NO_POSIX_SIGNALS
    -DCATCH_CONFIG_COLOUR_ANSI
    -DCATCH_CONFIG_CONSOLE_WIDTH=50)
add_executable(capture ${CAPTURE_SOURCE_FILES} ${CATCH_HEADER})
target_link_libraries(capture 3ds::ctrulib)
add_3dsx_target(capture)
add_cia_target(capture Application.rsf image.png sound.wav)
