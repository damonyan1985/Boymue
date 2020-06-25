cmake_minimum_required(VERSION 3.4.1)

project(gif)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wno-unused-parameter")
set(GIF_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../framework/thirdparty/gif")

set(GIF_SRC_LIST
  ${GIF_SRC}/dgif_lib.c
  ${GIF_SRC}/egif_lib.c
  ${GIF_SRC}/gifalloc.c
  ${GIF_SRC}/gif_err.c
  ${GIF_SRC}/gif_hash.c
  ${GIF_SRC}/quantize.c
)

add_library(
  gif
  SHARED
  ${GIF_SRC_LIST}
)

target_include_directories(
  gif
  PRIVATE
  ${GIF_SRC}
)
