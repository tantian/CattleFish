/*
 * Fish.cpp
 *
 *  Created on: 2015-9-1
 *      Author: guh
 */

#include "Fish.h"
#include "GameElement.h"

Fish* Fish::create(int type) {
	Fish *sprite = new (std::nothrow) Fish();
	sprite->m_whichtype = type;
	sprite->autorelease();
	return sprite;
}

Fish* Fish::createWithSpriteFrame(SpriteFrame *spriteFrame, int type) {
	Fish *sprite = new (std::nothrow) Fish();
	sprite->m_whichtype = type;
	if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
	{
		sprite->autorelease();
		sprite->runActionWithAnimation(sprite->m_whichtype);
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

Fish::Fish() {
	// TODO Auto-generated constructor stub

}

Fish::~Fish() {
	// TODO Auto-generated destructor stub
}

void Fish::setFishRoute(float dt) {
	int direction = abs(rand()%2), route = abs(rand()%2);
	auto winsize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto fishSize = this->getContentSize();

	float rand = rand_0_1();
	float randh1 = rand_0_1();
	float randh2 = rand_0_1();
	randh1 = randh1>0.5 ? randh1: randh1+0.45;
	randh2 = randh2>0.5 ? randh2: randh2+0.45;
//	if (route == 0) {
		linerRoute(Vec2(origin.x+winsize.width*rand-fishSize.width/2, origin.y+randh2*winsize.height-fishSize.height/2),
					dt, direction);
//	} else {
//		bezierRoute(Vec2(origin.x, origin.y+randh1*winsize.height),
//					Vec2(origin.x+winsize.width*rand-fishSize.width/2, origin.y+randh2*winsize.height-fishSize.height/2),
//					dt, direction);
//	}
}

void Fish::linerRoute(Vec2 to, float dt, bool direction) {
//	this->setFlipX(direction);
	Vec2 from = this->getPosition();

	float degree = MyRotate::rotate(to, from);
	float deldegree = degree - this->getRotation();
	if (deldegree > 180) {
		deldegree -= 360;
	} else if (deldegree < -180) {
		deldegree += 360;
	}
	int times = fabsf(deldegree)/30+1;
//	log("will degree is %f, deldegree is %f, fish rotate is %f", degree, deldegree, this->getRotation());
	auto moveto = MoveTo::create(dt, to);
	auto end = CallFuncN::create(this, callfuncN_selector(Fish::moveActionEnd));
	auto action = Sequence::create(
			RotateBy::create(0.1f*times, deldegree),
			moveto, end, NULL);
	auto repeat = Repeat::create(action, 1);
	repeat->setTag(FISH_MOVE_ACTION);
	this->runAction(repeat);
}

void Fish::bezierRoute(Vec2 to, float dt, bool direction) {
	Size s = Director::getInstance()->getVisibleSize();
	Vec2 from = this->getPosition();
	ccBezierConfig b1;
	b1.endPosition = to;
	Vec2 toVector = to - from;
	b1.controlPoint_1 = Vec2(from.x+toVector.x/3, from.y+toVector.y/4);
	log("1111111");
	b1.controlPoint_2 = Vec2(b1.controlPoint_1.x, b1.controlPoint_1.y+toVector.y/3);
	log("2222222");

	Vec2 normalizedVector = ccpNormalize(toVector);
	log("444");
	float radians = atan2(normalizedVector.y, - normalizedVector.x); //
	log("5555");
	float degree = CC_RADIANS_TO_DEGREES(radians); //
	log("6666");
	auto move = Sequence::create(
			Spawn::create(
					BezierTo::create(dt, b1),
					RotateBy::create(dt, degree)),
			NULL);
	log("777777");
//	float h[] = {55.0f, s.height-10.0f};
//	float r[] = {(h[1]-h[0])/2, -(h[1]-h[0])/2};
//
//	this->setFlipX(direction);
//
//	ccBezierConfig b1, b2;
//	b1.controlPoint_1 = b1.controlPoint_2 = Vec2(s.width/2+r[!direction]/3, h[direction]);
//	b1.endPosition = Vec2(s.width/2, h[0]+r[0]);
//
//	b2.controlPoint_1 = b2.controlPoint_2 = Vec2(s.width/2+r[direction]/3, h[!direction]);
//	b2.endPosition = Vec2(s.width/2+r[direction], h[!direction]);
//
//	auto move = Sequence::create(MoveTo::create(dt/2, Vec2(s.width/2+r[!direction], h[direction])),
//			Spawn::create(
//					BezierTo::create(dt/4, b1),
//					RotateBy::create(dt/4, -90)),
//			Spawn::create(
//					BezierTo::create(dt/4, b2),
//					BezierTo::create(dt/4, b1)
//			),
//			MoveTo::create(dt/2, to),
//			NULL);

	auto end = CallFuncN::create(this, callfuncN_selector(Fish::moveActionEnd));
	log("88888");
	auto action = Sequence::create(move, end, NULL);
	log("99999999");
	this->runAction(action);
	log("3333");
}

void Fish::moveActionEnd(Node* sender) {
	this->getActionManager()->removeActionByTag(FISH_MOVE_ACTION, this);
	this->setFishRoute(INTERVALDT);
}

void Fish::runActionWithAnimation(int actiontag) {
	 __String* fishNm = CCString::createWithFormat("fish_one");
	char szNm[100] = "fish_zero";
	if (m_whichtype == 1) {
		sprintf(szNm, "fish_one");
		fishNm = CCString::createWithFormat("fish_one");
	} else if (m_whichtype == 2) {
		sprintf(szNm, "fish_two");
		fishNm = CCString::createWithFormat("fish_two");
	} else if (m_whichtype == 3) {
		sprintf(szNm, "fish_three");
		fishNm = CCString::createWithFormat("fish_three");
	}
//	auto animation = ReadResources::animation(szNm, 10, 0.1);
	auto animation = ReadResources::animation(szNm, 10, szNm);
	auto repeat = CCRepeatForever::create(CCAnimate::create(animation));
	repeat->setTag(actiontag);
	this->runAction(repeat);
}
