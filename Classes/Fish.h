/*
 * Fish.h
 *
 *  Created on: 2015-9-1
 *      Author: guh
 *
 *  勇敢牛去捕鱼，湖中鱼的包装类
 */

#ifndef FISH_H_
#define FISH_H_

#include "cocos2d.h"
USING_NS_CC;

/**
@class Fish
@brief base class is Sprite.
@details 游戏界面鱼的类;
*/
class Fish: public Sprite {
public:
	/** 鱼每个动作的时间 */
	const static int INTERVALDT = 10.0f;

	/** 鱼动作的标志 */
	const static int FISH_MOVE_ACTION = 10;

	static Fish* create(int type);

	/** 创建一条鱼
	 * @param spriteFrame 鱼创建的显示的第一帧
	 * @param type 哪一个类鱼
	 * @return 鱼类的一个对象的引用
	 *  */
	static Fish* createWithSpriteFrame(SpriteFrame *spriteFrame, int type);

	virtual ~Fish();

	/**
	 *  @param actiontag 鱼儿动作的标记
	 */
	void runActionWithAnimation(int actiontag);

	/** 鱼的转动方向
	 * 	dt 转动的角度
	 * */
	void setFishRoute(float dt);

	/**
	 * 鱼直线运动
	 */
	void linerRoute(Vec2 to, float dt, bool direction);

	/** 鱼的曲线运动  */
	void bezierRoute(Vec2 to, float dt, bool direction);

	/** 鱼儿一个动作做完，的回调函数 */
	void moveActionEnd(Node* sender);

private:
	Fish();

	/**
	 * 哪一种鱼
	 */
	int m_whichtype = 0;


};

#endif /* FISH_H_ */
