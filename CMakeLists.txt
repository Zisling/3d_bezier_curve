cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 14)

SET(PROJ_NAME1 "Game")
project(${PROJ_NAME1})

file(GLOB ENGINE Engine3D/*.cpp Engine3D/*.c  Engine3D/*.h)
file(GLOB DISPLAY DisplayGLFW/*.cpp DisplayGLFW/*.h)

include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/res/includes ${PROJECT_SOURCE_DIR}/Engine3D
 ${PROJECT_SOURCE_DIR}/DisplayGLFW)

file(COPY ${PROJECT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


add_library(Display ${DISPLAY})
add_library(Engine ${ENGINE})

add_subdirectory(${PROJECT_SOURCE_DIR}/Game)
add_subdirectory(${PROJECT_SOURCE_DIR}/Game2)
