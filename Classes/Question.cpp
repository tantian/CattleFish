/*
 * Question.cpp
 *
 *  Created on: 2015-7-14
 *      Author: guh
 */

#include "Question.h"
#include "jni.h"
#include "cocos2d.h"

Question::Question() {
	this->m_done = false;
}

Question::Question(string subject, string opn0, string opn1, string opn2, string opn3, int rightId) {
	mSubject = subject;
	mOptions0 = opn0;
	mOptions1 = opn1;
	mOptions2 = opn2;
	mOptions3 = opn3;
	mRightId = rightId;
	this->m_done = false;
}

Question::~Question() {
	// TODO Auto-generated destructor stub
}

Question* Question::create(string subject, string opn0, string opn1, string opn2, string opn3, int rightId) {
	auto ret = new (std::nothrow) Question(subject, opn0, opn1, opn2, opn3, rightId);
//	auto ret = new (std::nothrow) Question();
	ret->autorelease();
	return ret;
}

Question* Question::create(string subject, string opn0, string opn1, string opn2, int rightId) {
	auto ret = new (std::nothrow) Question(subject, opn0, opn1, opn2, NULL, rightId);
//	auto ret = new (std::nothrow) Question();
	ret->autorelease();
	return ret;
}

Question* Question::create() {
	auto ret = new (std::nothrow) Question();
	ret->autorelease();
	return ret;
}

string Question::getOptions0() const {
	return mOptions0;
}

void Question::setOptions0(string options0) {
	mOptions0 = options0;
}

string Question::getOptions1() const {
	return mOptions1;
}

void Question::setOptions1(string options1) {
	mOptions1 = options1;
}

string Question::getOptions2() const {
	return mOptions2;
}

void Question::setOptions2(string options2) {
	mOptions2 = options2;
}

string Question::getOptions3() const {
	return mOptions3;
}

void Question::setOptions3(string options3) {
	mOptions3 = options3;
}

int Question::getRightId() const {
	return mRightId;
}

void Question::setRightId(int rightI) {
	mRightId = rightI;
}

string Question::getSubject() const {
	return mSubject;
}

void Question::setSubject(string subject) {
	mSubject = subject;
}

void Question::setOption(string option, int which) {
	switch(which) {
	case 0:
		setOptions0(option);
		break;
	case 1:
		setOptions1(option);
		break;
	case 2:
		setOptions2(option);
		break;
	case 3:
		setOptions3(option);
		break;
	default:
		break;
	}
}

string Question::getOption(int which) const {
	string reback = getOptions0();

	switch(which) {
	case 1:
		reback = getOptions1();
		break;
	case 2:
		reback = getOptions2();
		break;
	case 3:
		reback = getOptions3();
		break;
	default:
		break;
	}
	return reback;
}

void Question::setChoice(int which) {
	mChoice = which;
}

int Question::getChoice() const{
	return mChoice;
}

bool Question::getDone() const{
	return m_done;
}
void Question::setDone(bool done) {
	m_done = done;
}


