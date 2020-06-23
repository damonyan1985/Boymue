cmake_minimum_required(VERSION 3.4.1)

project(jpeg)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wno-unused-parameter")

add_definitions(-O3 -DANDROID -DANDROID_TILE_BASED_DECODE -DENABLE_ANDROID_NULL_CONVERT)

set(JPEG_SRC_LIST
jaricom.c
jcapimin.c
jcapistd.c
jcarith.c
jccoefct.c
jccolor.c
jcdctmgr.c
jchuff.c
jcinit.c
jcmainct.c
jcmarker.c
jcmaster.c
jcomapi.c
jcparam.c
jcprepct.c
jcsample.c
jctrans.c 
jdapimin.c
jdapistd.c 
jdarith.c 
jdatadst.c 
jdatasrc.c
jdcoefct.c 
jdcolor.c
jddctmgr.c 
jdhuff.c 
jdinput.c 
jdmainct.c
jdmarker.c 
jdmaster.c
jdmerge.c 
jdpostct.c 
jdsample.c 
jdtrans.c 
jerror.c 
jfdctflt.c
jfdctfst.c 
jfdctint.c 
jidctflt.c 
jidctfst.c 
jidctint.c 
jquant1.c
jquant2.c 
jutils.c 
jmemmgr.c 
jmemnobs.c
)

add_library(
    jpeg
    SHARED
    ${JPEG_SRC_LIST}
)

target_include_directories(
    jpeg
    PRIVATE
    .
    )