cmake_minimum_required(VERSION 3.12)

project(Valando)

# Set Standar
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Find dependences' libraries and includes
if(WIN32)
    set(GLAD_DIR glad-win32)
    add_link_options(-mwindows)
endif()
if(APPLE)
    set(GLAD_DIR glad-macos)
endif()
find_package(glfw3 REQUIRED)
find_package(freetype REQUIRED)

include_directories(${GLAD_DIR}/include)
include_directories(${glfw3_DIR}/../../../include)

# Build glad
add_library(glad glad-win32/src/glad.c)

# Build project
add_executable(Valando main.cpp)
target_link_libraries(Valando glad glfw ${FREETYPE_LIBRARIES})



