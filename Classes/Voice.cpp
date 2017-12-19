/*
 * Voice.cpp
 *
 *  Created on: 2015-12-14
 *      Author: guh
 */

#include "Voice.h"

Voice::Voice() {
	// TODO Auto-generated constructor stub

}

Voice::~Voice() {
	// TODO Auto-generated destructor stub
}


void Voice::onStart(const char *data) {
//	JniMethodInfo jmi;
//	const char * jniClass = "com/readboy/cattlefish/MainActivity";
//	//获取java部分的方法,详细参数请参考JniMethodInfo类的源码，参数分别为，类对象，java类，java类的方法，方法的参数和返回值，
//	//V表示void类型返回值（即无返回值，括号里面的表示参数，参数是一个字符串【详见JNI签名规范】）
//	bool isHave = JniHelper::getStaticMethodInfo(jmi, jniClass, "voiceStart", "(Ljava/lang/String;)V");
//	if (isHave) {
//		//转换字符串以及格式
//		jstring jdata = jmi.env->NewStringUTF(GBKToUTF8(data));
//		//调用java的方法，前面2个参数是固定的，后面的参数是在java的方法中的参数
//		jmi.env->CallStaticVoidMethod(jmi.classID, jmi.methodID, jdata);
//	}
}

void Voice::onStop() {
//	JniMethodInfo jmi;
//	const char * jniClass = "com/readboy/cattlefish/MainActivity";
//	bool isHave = JniHelper::getStaticMethodInfo(jmi, jniClass, "voiceStop", "()V");
//	if (isHave) {
//		//调用java的方法，前面2个参数是固定的，后面的参数是在java的方法中的参数
//		jmi.env->CallStaticVoidMethod(jmi.classID, jmi.methodID);
//	}
}

