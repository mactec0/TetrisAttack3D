cmake_minimum_required(VERSION 2.8)
project(TetrisAttack3D)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories(${CMAKE_SOURCE_DIR}/common)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

file(GLOB SOURCES *.cpp)
add_executable(TetrisAttack3D  ${SOURCES})

target_link_libraries(TetrisAttack3D
    "-lGLU"
    "-lGL"
    "-lSDL2"
    "-lSDL2main"
	"-lGLEW")
