cmake_minimum_required(VERSION 3.4.1)

project(v8)

add_definitions(-DANDROID_NDK -DPOSIX -DV8_USE_EXTERNAL_STARTUP_DATA -DV8_TARGET_ARCH_ARM64=1 -D__ANDROID_API__=21)

set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -w -Wall -Wextra -frtti")
set(V8_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../framework/thirdparty/v8")

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
    v8_src_base
    "${V8_DIR}/src/base/platform/condition-variable.cc"
    "${V8_DIR}/src/base/platform/mutex.cc"
    "${V8_DIR}/src/base/platform/platform-posix.cc"
    "${V8_DIR}/src/base/platform/platform-linux.cc"
    "${V8_DIR}/src/base/platform/semaphore.cc"
    "${V8_DIR}/src/base/platform/time.cc"
    "${V8_DIR}/src/base/*.cc"
)

file(
    GLOB
    v8_gen
    "${V8_DIR}/gen/*.cc"
)

file(
    GLOB
    v8_snapshot
    ${V8_DIR}/src/snapshot/code-serializer.cc
    ${V8_DIR}/src/snapshot/deserializer.cc
    ${V8_DIR}/src/snapshot/natives-common.cc
    ${V8_DIR}/src/snapshot/partial-serializer.cc
    ${V8_DIR}/src/snapshot/serializer.cc
    ${V8_DIR}/src/snapshot/serializer-common.cc
    ${V8_DIR}/src/snapshot/snapshot-common.cc
    ${V8_DIR}/src/snapshot/snapshot-empty.cc
    ${V8_DIR}/src/snapshot/snapshot-source-sink.cc
    ${V8_DIR}/src/snapshot/startup-serializer.cc
)

file(
    GLOB
    v8_src
    "${V8_DIR}/src/*.cc"
    "${V8_DIR}/src/asm/arm64/*.cc"
    "${V8_DIR}/src/ast/*.cc"
    "${V8_DIR}/src/base/utils/*.cc"
    "${V8_DIR}/src/compiler/arm64/*.cc"
    "${V8_DIR}/src/compiler/*.cc"
    "${V8_DIR}/src/crankshaft/*.cc"
    "${V8_DIR}/src/crankshaft/arm64/*.cc"
    "${V8_DIR}/src/debug/*.cc"
    "${V8_DIR}/src/debug/arm64/*.cc"
    "${V8_DIR}/src/extensions/*.cc"
    "${V8_DIR}/src/full-codegen/*.cc"
    "${V8_DIR}/src/full-codegen/arm64/*.cc"
    "${V8_DIR}/src/heap/*.cc"
    "${V8_DIR}/src/ic/*.cc"
    "${V8_DIR}/src/ic/arm64/*.cc"
    "${V8_DIR}/src/interpreter/*.cc"
    "${V8_DIR}/src/libplatform/*.cc"
    "${V8_DIR}/src/parsing/*.cc"
    "${V8_DIR}/src/profiler/*.cc"
    "${V8_DIR}/src/regexp/*.cc"
    "${V8_DIR}/src/regexp/arm64/*.cc"
    "${V8_DIR}/src/runtime/*.cc"
    "${V8_DIR}/src/third_party/fdlibm/*.cc"
    "${V8_DIR}/src/third_party/vtune/*.cc"
    "${V8_DIR}/src/tracing/*.cc"
    "${V8_DIR}/src/wasm/*.cc"
    "${V8_DIR}/port/*.cc"
)

add_library(
    v8
    SHARED
    ${v8_src}
    ${v8_src_base}
    ${v8_gen}
    ${v8_snapshot}
)

target_include_directories(
    v8
    PRIVATE
    ${V8_DIR}
    ${V8_DIR}/include
    ${V8_DIR}/src
)

target_link_libraries(
    v8
    log
)