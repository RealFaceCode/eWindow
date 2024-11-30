cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME eWindowLib)
project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES C CXX)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS ON)

if($ENV{EWINDOWLIB_BUILD_EXAMPLES})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
endif()

include(FetchContent)

set(EUTIL_LIB_DIR "${CMAKE_SOURCE_DIR}/thirdparty/eUtil")
set(LOGCRAFT_LIB_DIR "${CMAKE_SOURCE_DIR}/thirdparty/LogCraft")
set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/thirdparty/glfw")

if(NOT EXISTS "${EUTIL_LIB_DIR}/CMakeLists.txt")
    message(STATUS "Library 'eUtil' not found. Fetching...")

    FetchContent_Declare(
        eUtil
        GIT_REPOSITORY https://github.com/RealFaceCode/eUtil.git
        GIT_TAG main
        SOURCE_DIR ${EUTIL_LIB_DIR}
    )

    if(NOT eUtil_POPULATED)
        FetchContent_Populate(eUtil)
    endif()

    message(STATUS "Library 'eUtil' fetched.")
else()
    message(STATUS "Library 'eUtil' already exists.")
    message(STATUS "Try to fetch updates...")

    execute_process(
        COMMAND git pull
        WORKING_DIRECTORY ${EUTIL_LIB_DIR}
    )
endif()

if(NOT EXISTS "${LOGCRAFT_LIB_DIR}/CMakeLists.txt")
    message(STATUS "Library 'LogCraft' not found. Fetching...")

    FetchContent_Declare(
        LogCraft
        GIT_REPOSITORY https://github.com/RealFaceCode/LogCraft.git
        GIT_TAG main
        SOURCE_DIR ${LOGCRAFT_LIB_DIR}
    )

    if(NOT LogCraft_POPULATED)
        FetchContent_Populate(LogCraft)
    endif()

    message(STATUS "Library 'LogCraft' fetched.")
else()
    message(STATUS "Library 'LogCraft' already exists.")
    message(STATUS "Try to fetch updates...")

    execute_process(
        COMMAND git pull
        WORKING_DIRECTORY ${LOGCRAFT_LIB_DIR}
    )
endif()

if(NOT EXISTS "${GLFW_LIB_DIR}/CMakeLists.txt")
    message(STATUS "Library 'glfw' not found. Fetching...")

    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG master
        SOURCE_DIR ${GLFW_LIB_DIR}
    )
    
    if(NOT glfw_POPULATED)
        FetchContent_Populate(glfw)
    endif()
    message(STATUS "Library 'glfw' fetched.")
    else()
    message(STATUS "Library 'glfw' already exists.")
    message(STATUS "Try to fetch updates...")

    execute_process(
        COMMAND git pull
        WORKING_DIRECTORY ${GLFW_LIB_DIR}
    )
endif()

if(NOT TARGET glfw)
    add_subdirectory(${GLFW_LIB_DIR} ${CMAKE_CURRENT_BINARY_DIR}/glfw)
endif()

if(NOT TARGET eUtil)
    add_subdirectory(${EUTIL_LIB_DIR} ${CMAKE_CURRENT_BINARY_DIR}/eUtil)
endif()

if(NOT TARGET LogCraft)
    add_subdirectory(${LOGCRAFT_LIB_DIR} ${CMAKE_CURRENT_BINARY_DIR}/LogCraft)
endif()

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS  "libBuild/src/*.cpp")

if(NOT TARGET ${PROJECT_NAME})
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
    target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libBuild/hdr
    )
    target_include_directories(${PROJECT_NAME} 
    PUBLIC ${LOGCRAFT_LIB_DIR}/libBuild/hdr
    PUBLIC ${EUTIL_LIB_DIR}/libBuild/hdr
    PUBLIC ${GLFW_LIB_DIR}/include
    PUBLIC thirdparty/stbi
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE LogCraft glfw eUtil)
    target_compile_definitions(${PROJECT_NAME} PRIVATE EWIN_EXPORTS)
endif()

message(STATUS "eWindow build examples: " $ENV{EWINDOWLIB_BUILD_EXAMPLES})

if($ENV{EWINDOWLIB_BUILD_EXAMPLES})
    add_subdirectory(buildtest)
endif()