cmake_minimum_required(VERSION 3.5)
project(eWindowLib VERSION 0.1.0 LANGUAGES C CXX)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

add_subdirectory(thirtparty/eLog)
add_subdirectory(thirtparty/glfw)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS  "libBuild/src/*.cpp")

if(NOT TARGET eWindowLib)
    add_library(eWindowLib SHARED ${SOURCE_FILES})
    target_compile_features(eWindowLib PRIVATE cxx_std_20)
    target_include_directories(eWindowLib PUBLIC libBuild/hdr)
    target_link_libraries(eWindowLib PRIVATE eLogLib glfw)
    target_include_directories(eWindowLib PRIVATE thirtparty/glfw/include thirtparty/eLog/libBuild/hdr)
endif()