cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wno-unused-parameter")

add_subdirectory(freetype)
add_subdirectory(jpeg)
add_subdirectory(png)
add_subdirectory(zlib)
add_subdirectory(gif)
add_subdirectory(expat)
add_subdirectory(skia)
add_subdirectory(v8)
add_subdirectory(qjs)