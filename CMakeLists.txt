cmake_minimum_required(VERSION 3.11)
project(robot2D_game)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")

option(GAME_MODE "Build game for playing" OFF)

#need for audio only just now
find_package(SFML 2.5 COMPONENTS audio REQUIRED)

find_package(glfw3 3.3 REQUIRED)
find_package(robot2D REQUIRED)
find_package(robot2D_ext REQUIRED)

set(BUILD_INIH TRUE)

if (BUILD_INIH)
    message(STATUS "+-Add ini-parser module dependency")
    include(cmake/External_Inih.cmake)
    message(STATUS "| Inih include dirs: ${Inih_INCLUDE_DIRS}")
    message(STATUS "| Inih libraries: ${Inih_LIBRARIES}")
else ()
    message("don't have find package ")
endif ()

include_directories(${Inih_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/incl)

file(GLOB_RECURSE SRC src/*.cpp)

set(LIBS robot2D sfml-audio)

add_executable(${PROJECT_NAME} ${SRC})
if (BUILD_INIH)
    add_dependencies(${PROJECT_NAME} Inih_ext)
    set(LIBS ${LIBS} ${Inih_LIBRARIES})
endif ()

target_link_libraries(${PROJECT_NAME} ${LIBS})

if (GAME_MODE)
    #todo copy resources libs and etc
    if (WIN32)
        #link options to remove console
        add_link_options(-mwindows)
    endif ()
endif ()