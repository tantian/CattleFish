/*
 * Question.h
 *
 *  Created on: 2015-7-14
 *      Author: guh
 */

#ifndef QUESTION_H_
#define QUESTION_H_

#include <stdlib.h>
#include "cocos2d.h"
#include "CCRef.h"

USING_NS_CC;
using namespace std;

/**
@class Question
@brief Question is a subclass of Ref that implements the reference count management.
@details This class included the stem, options, right answer ID, choice answer id and is done;
*/
class CC_DLL Question : public Ref {
public:
	Question();
	Question(string subject, string opn0, string opn1, string opn2, string opn3, int rightId);
	virtual ~Question();

	static Question* create();
	static Question* create(string subject, string opn0, string opn1, string opn2, int rightId);
	static Question* create(string subject, string opn0, string opn1, string opn2, string opn3, int rightId);


	string getOptions0() const;

	void setOptions0(string options0);

	string getOptions1() const ;

	void setOptions1(string options1) ;

	std::string getOptions2() const ;

	void setOptions2(string options2) ;

	string getOptions3() const ;

	void setOptions3(string options3) ;

	int getRightId() const ;

	void setRightId(int rightI = 0) ;

	string getSubject() const;

	void setSubject(string subject);

	void setOption(string option, int which);

	string getOption(int which) const;

	void setChoice(int which);

	int getChoice() const;

	bool getDone() const;
	void setDone(bool done);


private:
	string mSubject;
	string mOptions0;
	string mOptions1;
	string mOptions2;
	string mOptions3;
	int mRightId = 0;

	int mChoice = -1;

	bool m_done = false;
};

#endif /* QUESTION_H_ */
