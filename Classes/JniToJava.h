/*
 * JniToJava.h
 *
 *  Created on: 2015-7-13
 *      Author: guh
 */

#ifndef JNITOJAVA_H_
#define JNITOJAVA_H_

#include <stdlib.h>
#include "cocos2d.h"
#include "Question.h"

USING_NS_CC;
using namespace std;

/**
@class JniToJava
@brief JniToJava is a subclass of Ref that implements the reference count management.
@details this class is called java code;
*/
class CC_DLL JniToJava : public Ref
{
public:
	JniToJava();
	virtual ~JniToJava();

    /** Creates a JniToJava.
     *
     * @return An autoreleased JniToJava object.
     */
	static JniToJava* create();


	void showToastMsg(const char* msg);

    /** Callback function for get question data from java code.
     * @param which: the question number in this section.
     * @return a reference of Question object.
     * @js NA
     */
	Question* getTitleSubject(int which);

    /** Callback function for get json file path.
     * @return json file path. if the question was not existed, the subject and options of this question is "".
     * @js NA
     */
	std::string getJSONPath();

    /** Callback function for get the total number of topics.
     * @return the total number of topics.
     * @js NA
     */
	int getQuestionNumber();

    /** Callback function for notificition java save did errer question.
     * @param rightRate: The correct rate of this game.
     * @param errNumber: the nubmer of errer question what user has play game.
     * @param qstnumbers: A series of wrong topic number
     * @js NA
     */
	void saveWrongQsts(int rightRate, int errNumber, int qstnumbers[]);

	/** Callback function for notificition java save learn plan data.
	 * @param rightTitle: The right title number of user answer.
     * @param doCount: The right title number of user answer.
     * @param didTime: The time of user play game.
     * @param score: The score of user play game.
	 */
	void savePlanData(int rightTitle, int doCount, int didTime, int score);

	void voiceStart(int qstidx);

	void voiceStop();

	void release();
	void releaseRe();
	bool canSound(int qstidx);

};

#endif /* JNITOJAVA_H_ */
