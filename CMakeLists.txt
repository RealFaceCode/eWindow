cmake_minimum_required(VERSION 3.5)
project(eWindowLib VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS  "libBuild/src/*.cpp")

add_library(eWindowLib SHARED ${SOURCE_FILES})
target_compile_features(eWindowLib PRIVATE cxx_std_20)
target_include_directories(eWindowLib PUBLIC libBuild/hdr)
