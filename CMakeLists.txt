cmake_minimum_required(VERSION 3.12)

project(Valando)

if(WIN32)
    set(GLAD_DIR glad-win32)
endif()
find_package(glfw3 REQUIRED)

include_directories(${GLAD_DIR}/include)
include_directories(${glfw3_DIR}/../../../include)

add_library(glad glad-win32/src/glad.c)

add_executable(Valando main.cpp)
target_link_libraries(Valando glad glfw)



