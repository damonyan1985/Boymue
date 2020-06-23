LOCAL_PATH:= $(call my-dir)

# We need to build this for both the device (as a shared library)
# and the host (as a static library for tools to use).

include $(CLEAR_VARS)
#LOCAL_SRC_FILES := $(addprefix source/,$(CSOURCES))
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/

LOCAL_SRC_FILES += \
	$(LOCAL_PATH)/source/adler32.c \
	$(LOCAL_PATH)/source/compress.c \
	$(LOCAL_PATH)/source/crc32.c \
	$(LOCAL_PATH)/source/deflate.c \
	$(LOCAL_PATH)/source/gzclose.c \
	$(LOCAL_PATH)/source/gzlib.c \
	$(LOCAL_PATH)/source/gzread.c \
	$(LOCAL_PATH)/source/gzwrite.c \
	$(LOCAL_PATH)/source/infback.c \
	$(LOCAL_PATH)/source/inffast.c \
	$(LOCAL_PATH)/source/inflate.c \
	$(LOCAL_PATH)/source/inftrees.c \
	$(LOCAL_PATH)/source/trees.c \
	$(LOCAL_PATH)/source/uncompr.c \
	$(LOCAL_PATH)/source/zutil.c \

common_CFLAGS := -std=gnu89 -Wno-unused-parameter
LOCAL_CLANG := true
#LOCAL_SRC_FILES := $(common_SRC_FILES)
LOCAL_CFLAGS += $(common_CFLAGS) -ftrapv
LOCAL_MODULE:= z
include $(BUILD_SHARED_LIBRARY)