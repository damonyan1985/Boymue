cmake_minimum_required(VERSION 3.4.1)

project(expat)

add_definitions(-DHAVE_EXPAT_CONFIG_H)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wno-unused-parameter -Wmissing-prototypes -Wstrict-prototypes -Wno-missing-field-initializers -fexceptions")
set(EXPAT_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../framework/thirdparty/expat")

set(
  expat_src_list
  ${EXPAT_SRC}/lib/xmlparse.c
  ${EXPAT_SRC}/lib/xmlrole.c
  ${EXPAT_SRC}/lib/xmltok.c
)

add_library(
  expat
  SHARED
  ${expat_src_list}
)

target_include_directories(
  expat
  PRIVATE
  ${EXPAT_SRC}
  ${EXPAT_SRC}/lib
)