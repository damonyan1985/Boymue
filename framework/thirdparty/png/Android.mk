LOCAL_PATH:= $(call my-dir)

# We need to build this for both the device (as a shared library)
# and the host (as a static library for tools to use).

include $(CLEAR_VARS)
#LOCAL_SRC_FILES := $(addprefix source/,$(CSOURCES))
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/

LOCAL_SRC_FILES += \
	$(LOCAL_PATH)/source/png.c \
	$(LOCAL_PATH)/source/pngerror.c \
	$(LOCAL_PATH)/source/pngget.c \
	$(LOCAL_PATH)/source/pngmem.c \
	$(LOCAL_PATH)/source/pngpread.c \
	$(LOCAL_PATH)/source/pngread.c \
	$(LOCAL_PATH)/source/pngrio.c \
	$(LOCAL_PATH)/source/pngrtran.c \
	$(LOCAL_PATH)/source/pngrutil.c \
	$(LOCAL_PATH)/source/pngset.c \
	$(LOCAL_PATH)/source/pngtrans.c \
	$(LOCAL_PATH)/source/pngwio.c \
	$(LOCAL_PATH)/source/pngwrite.c \
	$(LOCAL_PATH)/source/pngwtran.c \
	$(LOCAL_PATH)/source/pngwutil.c \		

common_CFLAGS := -std=gnu89 -Wno-unused-parameter
LOCAL_CLANG := true
#LOCAL_SRC_FILES := $(common_SRC_FILES)
LOCAL_CFLAGS += $(common_CFLAGS) -ftrapv
#LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)
LOCAL_SHARED_LIBRARIES := libz
LOCAL_MODULE:= png
include $(BUILD_SHARED_LIBRARY)