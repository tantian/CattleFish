LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

####################### third so begin
include $(CLEAT_VARS)
LOCAL_MODULE := TutorGameData
LOCAL_SRC_FILES := prebuilt/libTutorGameData.so
include $(PREBUILT_SHARED_LIBRARY)

#include $(CLEAT_VARS)
#LOCAL_MODULE := Aisound
#LOCAL_SRC_FILES := prebuilt/libAisound.so
#include $(PREBUILT_SHARED_LIBRARY)
####################### third so end

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cattlefish_shared

LOCAL_MODULE_FILENAME := libcattlefish

FILE_LIST := hellocpp/main.cpp
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)  
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
