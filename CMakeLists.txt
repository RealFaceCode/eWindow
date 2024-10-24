cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME eWindowLib)
project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES C CXX)


set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

if($ENV{EWINDOWLIB_BUILD_EXAMPLES})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
endif()

add_subdirectory(thirdparty/LogCraft)
add_subdirectory(thirdparty/eUtil)
add_subdirectory(thirdparty/glfw)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS  "libBuild/src/*.cpp")

if(NOT TARGET ${PROJECT_NAME})
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
    target_include_directories(${PROJECT_NAME} PUBLIC libBuild/hdr )
    target_include_directories(${PROJECT_NAME} PRIVATE thirdparty/LogCraft/libBuild/hdr thirdparty/eUtil/libBuild/hdr thirdparty/glfw/include thirdparty/stbi)
    target_link_libraries(${PROJECT_NAME} PRIVATE LogCraft glfw eUtil)
    target_compile_definitions(${PROJECT_NAME} PRIVATE EWIN_EXPORTS)
endif()

message(STATUS "eWindow build examples: " $ENV{EWINDOWLIB_BUILD_EXAMPLES})

if($ENV{EWINDOWLIB_BUILD_EXAMPLES})
    add_subdirectory(buildtest)
endif()