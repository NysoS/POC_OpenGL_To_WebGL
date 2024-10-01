cmake_minimum_required(VERSION 3.10)

project("POC_OpenGL_TO_WebGL")
add_subdirectory(POC_OpenGL_To_WebGL)

include("${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT)