cmake_minimum_required(VERSION 3.4.1)

project(png)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wno-unused-parameter")

set(PNG_SRC_LIST
source/png.c
source/pngerror.c
source/pngget.c
source/pngmem.c
source/pngpread.c
source/pngread.c
source/pngrio.c
source/pngrtran.c
source/pngrutil.c
source/pngset.c
source/pngtrans.c
source/pngwio.c
source/pngwrite.c
source/pngwtran.c
source/pngwutil.c
)

add_library(
    png
    SHARED
    ${PNG_SRC_LIST}
)

target_include_directories(
    png
    PRIVATE
    include
    )

target_link_libraries(
    png
    zlib
)   