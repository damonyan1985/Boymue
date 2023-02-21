cmake_minimum_required(VERSION 3.4.1)

project(qjs)

add_definitions(-DANDROID_NDK -DPOSIX -D__ANDROID_API__=21)

set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -w -Wall -Wextra -frtti")
set(QJS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../framework/thirdparty/quickjs")

if (${ANDROID_ABI} STREQUAL "x86_64")
    include_directories(${ANDROID_SYSROOT}/usr/include/x86_64-linux-android)
elseif (${ANDROID_ABI} STREQUAL "x86")
    include_directories(${ANDROID_SYSROOT}/usr/include/i686-linux-android)
elseif (${ANDROID_ABI} STREQUAL "arm64-v8a")
    include_directories(${ANDROID_SYSROOT}/usr/include/aarch64-linux-android)
elseif (${ANDROID_ABI} STREQUAL "armeabi-v7a")
    include_directories(${ANDROID_SYSROOT}/usr/include/arm-linux-androideabi)
endif()

file(
    GLOB
    qjs_src
    ${QJS_DIR}/src/cutils.c
    ${QJS_DIR}/src/libbf.cc
    ${QJS_DIR}/src/libregexp.cc
    ${QJS_DIR}/src/libunicode.cc
    ${QJS_DIR}/src/quickjs.cc
    ${QJS_DIR}/src/quickjs-libc.cc
)

add_library(
    qjs
    SHARED
    ${qjs_src}
)

target_include_directories(
    qjs
    PRIVATE
    ${QJS_DIR}
)

target_link_libraries(
    qjs
    log
)