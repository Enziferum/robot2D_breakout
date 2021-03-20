cmake_minimum_required(VERSION 3.12)
project(robot2D_game)

set(CMAKE_CXX_STANDARD 14)

#need for audio only just now
find_package(SFML 2.5 COMPONENTS audio REQUIRED)

#window backend to
find_package(glfw3 3.3 REQUIRED)
find_package(robot2D REQUIRED)
#glad stb_image robot2D_audio ( new )
find_package(robot2D_ext REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/incl)
file(GLOB SRC src/*.cpp)

set(LIBS robot2D sfml-audio)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} ${LIBS})