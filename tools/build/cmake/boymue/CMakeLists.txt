cmake_minimum_required(VERSION 3.4.1)

project(boymue)

add_definitions(-DANDROID_NDK -DANDROID -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=1 -DSKIA_IMPLEMENTATION=1)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Wno-unused-parameter")
set(BOYMUE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../../framework/boymue")
set(THIRDPARTY_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../../framework/thirdparty")

if(${ANDROID_ABI} STREQUAL "armeabi-v7a")
	include_directories(${ANDROID_SYSROOT}/usr/include/arm-linux-androideabi)
elseif(${ANDROID_ABI} STREQUAL "arm64-v8a")
	include_directories(${ANDROID_SYSROOT}/usr/include/aarch64-linux-android)
else()
	include_directories(${ANDROID_SYSROOT}/usr/include/arm-linux-androideabi)
endif()

set(
  port_src_list
  ${BOYMUE_SRC}/port/SkiaPort.cpp
)

file(
  GLOB 
  platform_src_list
  ${BOYMUE_SRC}/platform/android/gpu/*.cpp
)

add_library(
  boymue
  SHARED
  ${port_src_list}
  ${platform_src_list}
)

target_link_libraries(
  boymue
  skia
  jnigraphics
  EGL
  GLESv2
) 

target_include_directories(
  boymue
  PRIVATE
  ${THIRDPARTY_SRC}/skia/include/config
  ${THIRDPARTY_SRC}/skia/include/core
  ${BOYMUE_SRC}/platform/android/gpu
)