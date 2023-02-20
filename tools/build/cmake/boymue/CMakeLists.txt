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

file(
  GLOB
  port_src_list
  ${BOYMUE_SRC}/port/SkiaPort.cpp
  ${BOYMUE_SRC}/port/android/*.cpp
)

file(
  GLOB
  core_src_list
  ${BOYMUE_SRC}/core/paint/*.cpp
  ${BOYMUE_SRC}/core/view/*.cpp
  ${BOYMUE_SRC}/core/layout/*.cpp
  ${BOYMUE_SRC}/core/dom/*.cpp
  ${BOYMUE_SRC}/thread/*.cpp
  ${BOYMUE_SRC}/thread/event/*.cpp
  ${BOYMUE_SRC}/thread/task/*.cpp
  ${BOYMUE_SRC}/thread/time/*.cpp
  ${BOYMUE_SRC}/app/*.cpp
  ${BOYMUE_SRC}/util/*.cpp
  ${BOYMUE_SRC}/loader/*.cpp
  ${BOYMUE_SRC}/engine/*.cpp
  ${BOYMUE_SRC}/apis/*.cpp
)

file(
  GLOB 
  platform_src_list
  ${BOYMUE_SRC}/platform/android/*.cpp
  ${BOYMUE_SRC}/platform/android/gpu/*.cpp
)

add_library(
  boymue
  SHARED
  ${port_src_list}
  ${platform_src_list}
  ${core_src_list}
)

target_link_libraries(
  boymue
  skia
  v8
  expat
  jnigraphics
  EGL
  GLESv2
  android
) 

target_include_directories(
  boymue
  PRIVATE
  ${THIRDPARTY_SRC}
  ${THIRDPARTY_SRC}/skia/include/config
  ${THIRDPARTY_SRC}/skia/include/core
  ${THIRDPARTY_SRC}/skia/include/gpu
  ${THIRDPARTY_SRC}/skia/include/utils
  ${THIRDPARTY_SRC}/rapidjson/include
  ${THIRDPARTY_SRC}/rapidxml/include
  ${THIRDPARTY_SRC}/bmnet/bmnet-main/include
  ${THIRDPARTY_SRC}/v8
  ${THIRDPARTY_SRC}/v8/include
  ${THIRDPARTY_SRC}/expat
  ${THIRDPARTY_SRC}/expat/lib
  ${BOYMUE_SRC}/platform/android/gpu
  ${BOYMUE_SRC}/platform/android
  ${BOYMUE_SRC}/platform
  ${BOYMUE_SRC}/port/android
  ${BOYMUE_SRC}/core/view
  ${BOYMUE_SRC}/core/paint
  ${BOYMUE_SRC}/core/layout
  ${BOYMUE_SRC}/core/style
  ${BOYMUE_SRC}/core/dom
  ${BOYMUE_SRC}/thread
  ${BOYMUE_SRC}/thread/event
  ${BOYMUE_SRC}/thread/task
  ${BOYMUE_SRC}/thread/time
  ${BOYMUE_SRC}/app
  ${BOYMUE_SRC}/engine
  ${BOYMUE_SRC}/loader
  ${BOYMUE_SRC}/util
  ${BOYMUE_SRC}/apis
)