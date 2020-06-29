cmake_minimum_required(VERSION 3.4.1)

project(quickjs)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wno-unused-parameter")
set(QUICKJS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../framework/thirdparty/quickjs")

set(
  quickjs_src_list
  ${QUICKJS_SRC}/bjson.c
  ${QUICKJS_SRC}/cutils.c
  ${QUICKJS_SRC}/jscompress.c
  ${QUICKJS_SRC}/libbf.c
  ${QUICKJS_SRC}/libregexp.c
  ${QUICKJS_SRC}/libunicode.c
  #${QUICKJS_SRC}/qjs.c
  #${QUICKJS_SRC}/qjsc.c
  ${QUICKJS_SRC}/quickjs.c
  ${QUICKJS_SRC}/quickjs-libc.c
  #${QUICKJS_SRC}/unicode_gen.c
)

add_library(
  quickjs
  SHARED
  ${quickjs_src_list}
)

target_include_directories(
  quickjs
  PRIVATE
  ${QUICKJS_SRC}
)



