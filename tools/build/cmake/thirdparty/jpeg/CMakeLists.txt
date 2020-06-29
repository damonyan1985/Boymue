cmake_minimum_required(VERSION 3.4.1)

project(jpeg)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wno-unused-parameter")

add_definitions(-O3 -DANDROID -DANDROID_TILE_BASED_DECODE -DENABLE_ANDROID_NULL_CONVERT -U__aarch64__ -U__ARM_HAVE_NEON)
set(JPEG_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../framework/thirdparty/jpeg")

set(JPEG_SRC_LIST
  ${JPEG_SRC}/jcapimin.c
  ${JPEG_SRC}/jcapistd.c
  ${JPEG_SRC}/jccoefct.c
  ${JPEG_SRC}/jccolor.c
  ${JPEG_SRC}/jcdctmgr.c
  ${JPEG_SRC}/jchuff.c
  ${JPEG_SRC}/jcinit.c
  ${JPEG_SRC}/jcmainct.c
  ${JPEG_SRC}/jcmarker.c
  ${JPEG_SRC}/jcmaster.c
  ${JPEG_SRC}/jcomapi.c
  ${JPEG_SRC}/jcparam.c
  ${JPEG_SRC}/jcphuff.c
  ${JPEG_SRC}/jcprepct.c
  ${JPEG_SRC}/jcsample.c
  ${JPEG_SRC}/jctrans.c 
  ${JPEG_SRC}/jdapimin.c
  ${JPEG_SRC}/jdapistd.c 
  ${JPEG_SRC}/jdatadst.c 
  ${JPEG_SRC}/jdatasrc.c
  ${JPEG_SRC}/jdcoefct.c 
  ${JPEG_SRC}/jdcolor.c
  ${JPEG_SRC}/jddctmgr.c 
  ${JPEG_SRC}/jdhuff.c 
  ${JPEG_SRC}/jdinput.c 
  ${JPEG_SRC}/jdmainct.c
  ${JPEG_SRC}/jdmarker.c 
  ${JPEG_SRC}/jdmaster.c
  ${JPEG_SRC}/jdmerge.c 
  ${JPEG_SRC}/jdphuff.c
  ${JPEG_SRC}/jdpostct.c 
  ${JPEG_SRC}/jdsample.c 
  ${JPEG_SRC}/jdtrans.c 
  ${JPEG_SRC}/jerror.c 
  ${JPEG_SRC}/jfdctflt.c
  ${JPEG_SRC}/jfdctfst.c 
  ${JPEG_SRC}/jfdctint.c
  ${JPEG_SRC}/jidctflt.c 
  ${JPEG_SRC}/jidctfst.c 
  ${JPEG_SRC}/jidctint.c 
  ${JPEG_SRC}/jidctred.c
  ${JPEG_SRC}/jquant1.c
  ${JPEG_SRC}/jquant2.c 
  ${JPEG_SRC}/jutils.c 
  ${JPEG_SRC}/jmemmgr.c 
  ${JPEG_SRC}/jmemnobs.c
)

add_library(
  jpeg
  SHARED
  ${JPEG_SRC_LIST}
)

target_include_directories(
  jpeg
  PRIVATE
  ${JPEG_SRC}/
)