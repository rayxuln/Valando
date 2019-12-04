cmake_minimum_required(VERSION 3.12)

project(Valando)

# Set Standar
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/output)
if(NOT EXISTS ${EXECUTABLE_OUTPUT_PATH})
    file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
endif()

# Detect compiler
message(STATUS "CXX compiler id: ${CMAKE_CXX_COMPILER_ID}")
if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
    set(COMPILER_IS_CLANG TRUE)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(COMPILER_IS_GNU TRUE)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    set(COMPILER_IS_MSVC TRUE)
endif()

# Detect configure type
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(DEBUG TRUE)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    set(RELEASE TRUE)
endif()

# Find dependences' libraries and includes
if(WIN32)
    message(STATUS "Win32 Platform")
    set(GLAD_DIR glad-win32)
    if(COMPILER_IS_MSVC)
        add_link_options("/subsystem:\"windows\"")
    else()
        add_link_options(-mwindows)
    endif()
endif()
if(APPLE)
    message(STATUS "APPLE Platform")
    set(GLAD_DIR glad-macos)
endif()
find_package(glfw3 REQUIRED)
find_package(freetype REQUIRED)

# Include dirs
include_directories(${GLAD_DIR}/include)
include_directories(${glfw3_DIR}/../../../include)
include_directories(${FREETYPE_INCLUDE_DIRS})

# Build glad lib
add_library(glad glad-win32/src/glad.c)

# Build stb image lib
add_library(stb stb/stb_image.cpp stb/stb_image_write.cpp)

# Build project executable
add_executable(Valando main.cpp)
get_target_property(MAIN_TARGET_NAME Valando NAME)
target_link_libraries(Valando glad glfw stb ${FREETYPE_LIBRARIES})

# Copy resource files
if(APPLE)
    set(EXECUTABLE_DIR ${EXECUTABLE_OUTPUT_PATH}/${MAIN_TARGET_NAME}.app/Contents/MacOS)
    set_target_properties(
            ${MAIN_TARGET_NAME}
            PROPERTIES
            MACOSX_BUNDLE TRUE
    )
    set(MACOSX_BUNDLE_RESOURCES_DIR ${EXECUTABLE_OUTPUT_PATH}/${MAIN_TARGET_NAME}.app/Contents/Resources)
    add_custom_command(
            TARGET ${MAIN_TARGET_NAME}
            POST_BUILD
            COMMAND_EXPAND_LISTS
            COMMAND ${CMAKE_COMMAND} -E
                    make_directory  ${MACOSX_BUNDLE_RESOURCES_DIR}
            COMMAND ${CMAKE_COMMAND} -E
                    copy_directory ${PROJECT_SOURCE_DIR}/resource ${MACOSX_BUNDLE_RESOURCES_DIR}
            COMMENT "Copying resource files... for APPLE MACOSX BUNDLE"
    )
endif()
if(WIN32)
    set(EXECUTABLE_DIR ${EXECUTABLE_OUTPUT_PATH})
    set(RESOURCES_DIR ${EXECUTABLE_OUTPUT_PATH}/resource)
    add_custom_command(
            TARGET ${MAIN_TARGET_NAME}
            POST_BUILD
            COMMAND_EXPAND_LISTS
            COMMAND ${CMAKE_COMMAND} -E
                    make_directory  ${RESOURCES_DIR}
            COMMAND ${CMAKE_COMMAND} -E
                    copy_directory ${PROJECT_SOURCE_DIR}/resource ${RESOURCES_DIR}
            COMMENT "Copying resource files... for WIN32"
    )
endif()


