/*
 * Voice.h
 *
 *  Created on: 2015-12-14
 *      Author: guh
 */

#ifndef VOICE_H_
#define VOICE_H_

#include <CCRef.h>

class Voice: public cocos2d::Ref {
public:
	Voice();
	virtual ~Voice();

	void onStart(const char * data);
	void onStop();
};

#endif /* VOICE_H_ */
