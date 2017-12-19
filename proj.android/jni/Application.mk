APP_STL := gnustl_static
APP_PLATFORM := android-14
APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -std=c++11 -fsigned-char
APP_LDFLAGS := -latomic
#默认情况下，NDK的编译系统根据 "armeabi" ABI生成机器代码。可以使用APP_ABI 来选择一个不同的ABI。   APP_ABI := armeabi armeabi-v7a x86
#APP_ABI := armeabi armeabi-v7a

ifeq ($(NDK_DEBUG),1)
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=1
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif
