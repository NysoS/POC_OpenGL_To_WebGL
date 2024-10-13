project(POC_OpenGL_To_WebGL C CXX)

file(GLOB_RECURSE SOURCES "*.h" "*.cpp" "*.c")

include_directories(SOURCES)

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
GroupSources(SOURCES)


if(EMSCRIPTEN)
else()
	
endif(EMSCRIPTEN)

find_package(OpenGL REQUIRED)
if(NOT EMSCRIPTEN)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG)
endif()

find_package(SDL2 CONFIG REQUIRED)

set(EMSCRIPTEN_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/wasm32-emscripten/lib)
set(EMSCRIPTEN_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/wasm32-emscripten/include)

set({EXECUTABLE_OUTPUT_PATH} ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME} ${SOURCES})

if(EMSCRIPTEN)
	message("emscripten")
	set(VCPKG_TARGET_ARCHITECTURE wasm32-emscripten)
	set(CMAKE_TOOLCHAIN_FILE $ENV{EMSDK_HOME}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)
	set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")

	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-sUSE_GLFW=3 -sFULL_ES3=1 -sWASM=1 -sUSE_SDL=2 -sUSE_SDL_MIXER=2 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sNO_DISABLE_EXCEPTION_CATCHING --preload-file shader")
	
	target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
    	target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})
else()
	target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw)
endif()


target_link_libraries(${PROJECT_NAME} PRIVATE	
		SDL2::SDL2main
		SDL2::SDL2
	)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)