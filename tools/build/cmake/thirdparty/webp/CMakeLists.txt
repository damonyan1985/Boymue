cmake_minimum_required(VERSION 3.4.1)

project(webp)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wno-unused-parameter")
set(WEBP_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../framework/thirdparty/webp")

set(
  dec_src_list
  ${WEBP_SRC}/src/dec/bits.c
	${WEBP_SRC}/src/dec/dsp.c
	${WEBP_SRC}/src/dec/frame.c
	${WEBP_SRC}/src/dec/idec.c
	${WEBP_SRC}/src/dec/quant.c
	${WEBP_SRC}/src/dec/tree.c
	${WEBP_SRC}/src/dec/vp8.c
	${WEBP_SRC}/src/dec/webp.c
	${WEBP_SRC}/src/dec/yuv.c
)

set(
  enc_src_list
  ${WEBP_SRC}/src/enc/analysis.c
	${WEBP_SRC}/src/enc/bit_writer.c
	${WEBP_SRC}/src/enc/config.c
	${WEBP_SRC}/src/enc/cost.c
	${WEBP_SRC}/src/enc/dsp.c
	${WEBP_SRC}/src/enc/filter.c
	${WEBP_SRC}/src/enc/frame.c
	${WEBP_SRC}/src/enc/iterator.c
	${WEBP_SRC}/src/enc/picture.c
	${WEBP_SRC}/src/enc/quant.c
	${WEBP_SRC}/src/enc/syntax.c
	${WEBP_SRC}/src/enc/tree.c
	${WEBP_SRC}/src/enc/webpenc.c
)

add_library(
  webp
  SHARED
  {dec_src_list}
  {enc_src_list}
)

target_link_libraries(
  webp
  ${WEBP_SRC}/src/dec
  ${WEBP_SRC}/src/enc
  ${WEBP_SRC}/include
)  