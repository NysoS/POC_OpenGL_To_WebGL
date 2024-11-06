project(Loglib CXX)

file(GLOB_RECURSE SOURCES "*.h" "*.cpp")

set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(src)

macro(GroupSources curdir)
    file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir}
    ${PROJECT_SOURCE_DIR}/${curdir}/* ${PROJECT_SOURCE_DIR}/${curdir}/${curdir}/*)

    foreach(child ${children})

        if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
            GroupSources(${curdir}/${child})
        else()
            string(REPLACE "/" "\\" groupname ${curdir})
            source_group(${groupname} FILES
            ${PROJECT_SOURCE_DIR}/${curdir}/${child})
        endif()
    endforeach()
endmacro()

# Execute the macro
GroupSources(src)

if(EMSCRIPTEN)
    #add_library(${PROJECT_NAME} STATIC ${SOURCES}) #Link with a static library
    add_library(${PROJECT_NAME} SHARED ${SOURCES}) #Link with a dynamic library

    set(CMAKE_EXECUTABLE_SUFFIX ".wasm")

    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Os -s SIDE_MODULE=1")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Os -s WASM=1 -s SIDE_MODULE=1 -s STANDALONE_WASM --no -s EXPORT_ALL=1")

else()
    add_library(${PROJECT_NAME} SHARED ${SOURCES})

    add_compile_definitions(LOG_BUILD_DLL)
endif()

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

#if(EMSCRIPTEN)
#	set(VCPKG_TARGET_ARCHITECTURE wasm32-emscripten)
#	set(CMAKE_TOOLCHAIN_FILE $ENV{EMSDK_HOME}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)

#	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-sSIDE_MODULE=1 -sEXPORT_ALL -sWASM=1")
#endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)