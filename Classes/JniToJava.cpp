/*
 * JniToJava.cpp
 *
 *  Created on: 2015-7-13
 *      Author: guh
 */

#include "cocos2d.h"
#include "jni.h"
#include "../cocos2d/cocos/platform/android/jni/JniHelper.h"
#include "JniToJava.h"
#include "GameElement.h"

#define CLASS_NAME "com/readboy/cocos2dx/jni/ReadGameData"

JniToJava::JniToJava() {
	// TODO Auto-generated constructor stub

}

JniToJava::~JniToJava() {
	// TODO Auto-generated destructor stub
}

JniToJava* JniToJava::create()
{
    auto ret = new (std::nothrow) JniToJava();
    ret->autorelease();

    return ret;
}

void JniToJava::showToastMsg(const char* msg) {
	JniMethodInfo t;

   if (JniHelper::getStaticMethodInfo(t, "com/readboy/cattlefish/MainActivity", "saveWrongData", "(Ljava/lang/String;)V")) {
	   jstring jstr = t.env->NewStringUTF(msg);
       t.env->CallStaticVoidMethod(t.classID, t.methodID, jstr);
       t.env->DeleteLocalRef(t.classID);
       t.env->DeleteLocalRef(jstr);
   }
}

Question* JniToJava::getTitleSubject(int which) {
	JniMethodInfo t;
	std::string ret("JniToJava");
	std::string ret1("JniToJava");
	std::string ret2("JniToJava");
	Question* qst = Question::create();

	if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "qst", "(I)[Ljava/lang/String;")) {
		jobjectArray jobjarry = (jobjectArray)t.env->CallStaticObjectMethod(t.classID, t.methodID, which);

		int lenght = t.env->GetArrayLength(jobjarry);
		jstring jstr = (jstring)t.env->GetObjectArrayElement(jobjarry, 0);
		const char* chars = t.env->GetStringUTFChars(jstr, nullptr);

//		log(" option subject %s ===== ", chars);
		ret = JniHelper::jstring2string(jstr);
		ret1 = ret;
		qst->setSubject(ret);
		t.env->DeleteLocalRef(jstr);

		auto optionsrand = MyRandom::randomOrder(3);
		int whichoption = 0;
		for (int i=0; i<lenght-1; i++) {
			jstr = (jstring)t.env->GetObjectArrayElement(jobjarry, i+1);
			const char* chars = t.env->GetStringUTFChars(jstr, nullptr);
//			log(" option answer %s ", chars);
			ret = JniHelper::jstring2string(jstr);
			ret2.append(ret);
			ret2.append("-");
			t.env->DeleteLocalRef(jstr);
			whichoption = optionsrand[i];
			qst->setOption(ret, whichoption);
		}
		log("xuhao %d question %s answer %s", which, ret1.c_str(), ret2.c_str());
		qst->setRightId(optionsrand[0]);

		t.env->DeleteLocalRef(jobjarry);
		delete optionsrand;
	}

	return qst;
}


int JniToJava::getQuestionNumber() {
	JniMethodInfo t;
	std::string ret("JniToJava");
	int qstnumber = 0;
//	log(" JniToJava qst number is %d ",qstnumber);
	if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "qstNumber", "()I")) {
		int jobj = t.env->CallStaticIntMethod(t.classID, t.methodID);
		qstnumber = jobj;
//		log(" JniToJava qst number is %d ",jobj);
	}
	return qstnumber;
}

std::string JniToJava::getJSONPath() {
	std::string jsonPath("JniToJava");
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "path", "()Ljava/lang/String;")) {
		jstring jobj = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		jsonPath = JniHelper::jstring2string(jobj);

	}

	return jsonPath;
}


void JniToJava::saveWrongQsts(int rightRate, int errNumber, int qstnumbers[]) {
	JniMethodInfo t;
	std::string ret("JniToJava");
	int qstnumber = 0;
	if(JniHelper::getStaticMethodInfo(t, "com/readboy/cattlefish/MainActivity", "saveWrongData", "(I[I)Z")) {
	    jint buf[]={0, 1, 2};
	    for (int i=0; i<errNumber; i++) {
	    	log(" JniToJava errNumber is %d ", qstnumbers[i]);
	    }
	    jintArray jintArr; //定义jint数组
	    jintArr = t.env->NewIntArray(errNumber);
	    t.env->SetIntArrayRegion(jintArr, 0, errNumber, qstnumbers);
		t.env->CallStaticBooleanMethod(t.classID, t.methodID, rightRate, jintArr);
	}
}
void JniToJava::releaseRe(){
	log("GameScene release232() !!! ");
	JniMethodInfo t;
	std::string ret("JniToJava");
	int qstnumber = 0;
	if(JniHelper::getStaticMethodInfo(t, "com/readboy/cattlefish/MainActivity", "releaseResourse","()V")){
		log("GameScene release2322() !!! ");
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
	}

}

void JniToJava::savePlanData(int rightTitle, int doCount, int didTime, int score) {
	JniMethodInfo t;
	std::string ret("JniToJava");
	int qstnumber = 0;
	if(JniHelper::getStaticMethodInfo(t, "com/readboy/cattlefish/MainActivity", "savePlanData", "(IIII)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, rightTitle, doCount, didTime, score);
	}
	jint tt = 0;
	int at = tt;
}

void JniToJava::voiceStart(int qstidx) {
	JniMethodInfo t;
	std::string ret("JniToJava");
	int qstnumber = 0;
	if(JniHelper::getStaticMethodInfo(t, "com/readboy/cattlefish/MainActivity", "voiceStart", "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, qstidx);
	}
}

void JniToJava::voiceStop() {
	JniMethodInfo t;
	std::string ret("JniToJava");
	int qstnumber = 0;
	if(JniHelper::getStaticMethodInfo(t, "com/readboy/cattlefish/MainActivity", "voiceStop", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
	}
}

void JniToJava::release() {
	log("GameScene release() !!! ");
	JniMethodInfo t;
	std::string ret("JniToJava");
	int qstnumber = 0;
	if(JniHelper::getStaticMethodInfo(t, "com/readboy/cattlefish/MainActivity", "release", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
	}
}

bool JniToJava::canSound(int qstidx) {
	bool isSound = false;
	JniMethodInfo t;
	std::string ret("JniToJava");
	int qstnumber = 0;
	if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "isTtsSound", "()Z")) {
		jboolean jsnd = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
		isSound = jsnd;
	}
	return isSound;
}
