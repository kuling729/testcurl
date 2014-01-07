
LOCAL_PATH := $(call my-dir)






include $(CLEAR_VARS)
LOCAL_MODULE := curl
LOCAL_SRC_FILES := ./curllib/libcurl.a
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
MY_CPP_LIST := $($(LOCAL_PATH)/tinyxml/*.cpp)
LOCAL_MODULE := tinyxml
LOCAL_MODULE_FILENAME := libtinyxml 
LOCAL_SRC_FILES := \
tinyxml.cpp\
tinyxmlerror.cpp\
tinyxmlparser.cpp\
tinystr.cpp
include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_CFLAGS := -D_GNU_SOURCE
LOCAL_CPPFLAGS := -frtti
LOCAL_MODULE:= xmltest
LOCAL_SRC_FILES := xmltest.cpp xmlController.cpp downloadController.cpp
# 使用的本地静态库
LOCAL_STATIC_LIBRARIES := libcurl libtinyxml

# 如果不包含这一句的话，会提示：__android_log_print 未定义
LOCAL_LDLIBS := -llog -lsqlite 


include $(BUILD_EXECUTABLE)
