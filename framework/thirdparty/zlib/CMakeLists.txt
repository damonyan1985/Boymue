cmake_minimum_required(VERSION 3.4.1)

project(zlib)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wno-unused-parameter")

file(
    GLOB 
    zlib_src
    "source/*.c"
)

add_library(
    zlib
    SHARED
    ${zlib_src}
)

target_include_directories(
    zlib
    PRIVATE
    include
    )