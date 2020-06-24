cmake_minimum_required(VERSION 3.4.1)

project(png)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wno-unused-parameter")
set(PNG_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../framework/thirdparty/png")

set(PNG_SRC_LIST
    ${PNG_SRC}/source/png.c
    ${PNG_SRC}/source/pngerror.c
    ${PNG_SRC}/source/pngget.c
    ${PNG_SRC}/source/pngmem.c
    ${PNG_SRC}/source/pngpread.c
    ${PNG_SRC}/source/pngread.c
    ${PNG_SRC}/source/pngrio.c
    ${PNG_SRC}/source/pngrtran.c
    ${PNG_SRC}/source/pngrutil.c
    ${PNG_SRC}/source/pngset.c
    ${PNG_SRC}/source/pngtrans.c
    ${PNG_SRC}/source/pngwio.c
    ${PNG_SRC}/source/pngwrite.c
    ${PNG_SRC}/source/pngwtran.c
    ${PNG_SRC}/source/pngwutil.c
)

add_library(
    png
    SHARED
    ${PNG_SRC_LIST}
)

target_include_directories(
    png
    PRIVATE
    ${PNG_SRC}/include
    )

target_link_libraries(
    png
    zlib
)   