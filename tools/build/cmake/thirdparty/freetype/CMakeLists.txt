cmake_minimum_required(VERSION 3.4.1)

project(freetype)

add_definitions(-DANDROID_NDK -DFT2_BUILD_LIBRARY=1)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wno-unused-parameter")
set(FREETYPE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../framework/thirdparty/freetype")

set(
 FT2_SRC_LIST
 ${FREETYPE_SRC}/source/autofit/autofit.c
 ${FREETYPE_SRC}/source/base/basepic.c
 ${FREETYPE_SRC}/source/base/ftapi.c
 ${FREETYPE_SRC}/source/base/ftbase.c
 ${FREETYPE_SRC}/source/base/ftbbox.c
 ${FREETYPE_SRC}/source/base/ftbitmap.c
 ${FREETYPE_SRC}/source/base/ftdbgmem.c
 ${FREETYPE_SRC}/source/base/ftdebug.c
 ${FREETYPE_SRC}/source/base/ftglyph.c
 ${FREETYPE_SRC}/source/base/ftinit.c
 ${FREETYPE_SRC}/source/base/ftpic.c
 ${FREETYPE_SRC}/source/base/ftstroke.c
 ${FREETYPE_SRC}/source/base/ftsynth.c
 ${FREETYPE_SRC}/source/base/ftsystem.c
 ${FREETYPE_SRC}/source/cff/cff.c
 ${FREETYPE_SRC}/source/pshinter/pshinter.c
 ${FREETYPE_SRC}/source/pshinter/pshglob.c
 ${FREETYPE_SRC}/source/pshinter/pshpic.c
 ${FREETYPE_SRC}/source/pshinter/pshrec.c
 ${FREETYPE_SRC}/source/psnames/psnames.c
 ${FREETYPE_SRC}/source/psnames/pspic.c
 ${FREETYPE_SRC}/source/raster/raster.c
 ${FREETYPE_SRC}/source/raster/rastpic.c
 ${FREETYPE_SRC}/source/sfnt/pngshim.c
 ${FREETYPE_SRC}/source/sfnt/sfntpic.c
 ${FREETYPE_SRC}/source/sfnt/ttbdf.c
 ${FREETYPE_SRC}/source/sfnt/ttkern.c
 ${FREETYPE_SRC}/source/sfnt/ttload.c
 ${FREETYPE_SRC}/source/sfnt/ttmtx.c
 ${FREETYPE_SRC}/source/sfnt/ttpost.c
 ${FREETYPE_SRC}/source/sfnt/ttsbit.c
 ${FREETYPE_SRC}/source/sfnt/sfobjs.c
 ${FREETYPE_SRC}/source/sfnt/ttcmap.c
 ${FREETYPE_SRC}/source/sfnt/sfdriver.c
 ${FREETYPE_SRC}/source/smooth/smooth.c
 ${FREETYPE_SRC}/source/smooth/ftspic.c
 ${FREETYPE_SRC}/source/truetype/truetype.c
 ${FREETYPE_SRC}/source/type1/t1driver.c
 ${FREETYPE_SRC}/source/cid/cidgload.c
 ${FREETYPE_SRC}/source/cid/cidload.c
 ${FREETYPE_SRC}/source/cid/cidobjs.c
 ${FREETYPE_SRC}/source/cid/cidparse.c
 ${FREETYPE_SRC}/source/cid/cidriver.c
 ${FREETYPE_SRC}/source/pfr/pfr.c
 ${FREETYPE_SRC}/source/pfr/pfrgload.c
 ${FREETYPE_SRC}/source/pfr/pfrload.c
 ${FREETYPE_SRC}/source/pfr/pfrobjs.c
 ${FREETYPE_SRC}/source/pfr/pfrsbit.c
 ${FREETYPE_SRC}/source/type42/t42objs.c
 ${FREETYPE_SRC}/source/type42/t42parse.c
 ${FREETYPE_SRC}/source/type42/type42.c
 ${FREETYPE_SRC}/source/winfonts/winfnt.c
 ${FREETYPE_SRC}/source/pcf/pcfread.c
 ${FREETYPE_SRC}/source/pcf/pcfutil.c
 ${FREETYPE_SRC}/source/pcf/pcfdrivr.c
 ${FREETYPE_SRC}/source/psaux/afmparse.c
 ${FREETYPE_SRC}/source/psaux/psaux.c
 ${FREETYPE_SRC}/source/psaux/psconv.c
 ${FREETYPE_SRC}/source/psaux/psobjs.c
 ${FREETYPE_SRC}/source/psaux/t1decode.c
 ${FREETYPE_SRC}/source/tools/apinames.c
 ${FREETYPE_SRC}/source/type1/t1afm.c
 ${FREETYPE_SRC}/source/type1/t1gload.c
 ${FREETYPE_SRC}/source/type1/t1load.c
 ${FREETYPE_SRC}/source/type1/t1objs.c
 ${FREETYPE_SRC}/source/type1/t1parse.c
 ${FREETYPE_SRC}/source/bdf/bdfdrivr.c
 ${FREETYPE_SRC}/source/bdf/bdflib.c
 ${FREETYPE_SRC}/source/gzip/ftgzip.c
 ${FREETYPE_SRC}/source/lzw/ftlzw.c
)

add_library(
    freetype
    SHARED
    ${FT2_SRC_LIST}
)

target_include_directories(
    freetype
    PRIVATE
    ${FREETYPE_SRC}/include
    )