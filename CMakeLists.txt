cmake_minimum_required(VERSION 3.12)
project(hakka_game)

set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(SFML 2.5 COMPONENTS audio REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/incl)
file(GLOB SRC src/*.cpp)

find_path(
        GLAD_INCLUDES_DIR
        glad
        PATH_SUFFIXES
        incl
        PATHS
        ${CMAKE_BINARY_DIR}/third-party/glad-build/
)
include_directories(${GLAD_INCLUDES_DIR})

add_subdirectory(hakka)
set(LIBS hakka sfml-audio)

add_executable(${PROJECT_NAME} ${SRC} )
target_link_libraries(${PROJECT_NAME} ${LIBS})