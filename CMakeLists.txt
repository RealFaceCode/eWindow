cmake_minimum_required(VERSION 3.5)
project(eWindowLib VERSION 0.1.0 LANGUAGES C CXX)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

if($ENV{EWINDOWLIB_BUILD_EXAMPLES})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
endif()

add_subdirectory(thirdparty/eLog)
add_subdirectory(thirdparty/glfw)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS  "libBuild/src/*.cpp")

if(NOT TARGET eWindowLib)
    add_library(eWindowLib SHARED ${SOURCE_FILES})
    target_compile_features(eWindowLib PRIVATE cxx_std_20)
    target_include_directories(eWindowLib PUBLIC libBuild/hdr )
    target_include_directories(eWindowLib PRIVATE thirdparty/eLog/libBuild/hdr thirdparty/glfw/include thirdparty/stbi)
    target_link_libraries(eWindowLib PRIVATE eLogLib glfw)
endif()

message(STATUS "eWindowLib build examples: " $ENV{EWINDOWLIB_BUILD_EXAMPLES})

if($ENV{EWINDOWLIB_BUILD_EXAMPLES})
    add_subdirectory(buildtest)
endif()