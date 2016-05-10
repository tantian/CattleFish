#ifndef GAMEELEMENT_H_
#define GAMEELEMENT_H_

#include "cocos2d.h"
#include "Question.h"

USING_NS_CC;

//********************************************************* 牛 的类 Cattle **************************************************************//
class Cattle: public Sprite {
public:
	static const int NORMAL = 0x80;
	static const int HAPPY = 0x81;
	static const int ANGRY = 0x84;
	static const int SUC = 0x40;
	static const int FAIL = 0x41;
	static Cattle* create();
	static Cattle* createWithSpriteFrame(SpriteFrame *spriteFrame);

	Cattle();
	virtual ~Cattle();

	void runActionWithAnimation(Animation* animation, int actiontag);
	void runActionWithAnimation(int actiontag);
	void actionEndCallback(cocos2d::Ref* pSender);

private:

//	Animation* m_normal;
	bool m_is_jump = false;

};


//********************************************************* 答案字母的类 Leffter ***************************************************************//
class Leffter: public Sprite {
public:
	const static int AUTO_MOVE = 10;
	static Leffter* createWithSpriteFrame(SpriteFrame *spriteFrame, Vec2 original);

	Leffter();
	virtual ~Leffter();

    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	bool isTouchDowned(Vec2 touchPos);
	bool isUserMove(Vec2 touchPos);

	void runActionWithAnimation(Vec2 topos, const CallFuncN* callback);

	void reset();

	bool isMove();
	bool isAutoMove();

private:
	Vec2 m_original;
	bool m_touch_down = false;
	bool m_move = false;
	bool m_auto_move = false;

};




//************************************************************* 水波纹的类 WaterEffect *********************************************************//
class WaterEffect: public Sprite {
public:
	static WaterEffect* createWithSpriteFrame(SpriteFrame *spriteFrame);

	WaterEffect();
	virtual ~WaterEffect();

	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void runActionWithAnimation();
};



//********************************************************* 问题显示的类 QuestionLabel ********************************************************//
class QuestionLabel: public Label {
public:
    static QuestionLabel* createWithSystemFont(const Question* qst, const std::string& font, float fontSize,
        const Size& dimensions = Size::ZERO, TextHAlignment hAlignment = TextHAlignment::LEFT,
        TextVAlignment vAlignment = TextVAlignment::TOP);

	QuestionLabel(FontAtlas *atlas = nullptr, TextHAlignment hAlignment = TextHAlignment::LEFT,
		      TextVAlignment vAlignment = TextVAlignment::TOP,bool useDistanceField = false,bool useA8Shader = false);
	virtual ~QuestionLabel();

	/**
	 * 设置题目跟答案是否要空一行；在游戏界面的题目跟答案没有空一行间隔，游戏结束界面，题目跟答案显示有空一行显示
	 */
	void setSpaceLine(bool isSpaceLine);

	void setQuestion(int titlenumber, const Question* qst);

private:
	std::string qstToString(const Question* qst);
	bool is_line_qst_ansr = false;

};


//************************************************************ 炮架的类 Bogy ****************************************************************//
class Bogy: public Sprite {
public:
	const static int NORMAL = 0;
	const static int BOMB = 1;
	const static int LAUNCH = 2;
	static Bogy* createWithSpriteFrame(SpriteFrame *spriteFrame);

	Bogy();
	virtual ~Bogy();

	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);

	/**
	 * 答案是否有在这个炸弹位置
	 */
	bool isInView(Vec2 pos);

	void runActionWithAnimation(Vec2 pos, int state = NORMAL);

	/**
	 * 进入游戏用户首次答对题目，提醒用户捕鱼
	 */
	void runActionTip(Vec2 pos);

	void actionEndCallback(cocos2d::Ref* pSender, Vec2 pos);

	/**
	 * 创建捕鱼的子弹
	 */
	void createBullet(Vec2 pos, bool tip = false);

	void reset();

private:

	int m_create_net = 0;

};

//*********************************************************** 子弹的类 Bullet ************************************************************//
class Bullet: public Sprite {
public:
	static Bullet* createWithFileName(std::string name);

	Bullet();
	virtual ~Bullet();

	void runActionWithAnimation(Vec2 pos);

	void actionEndCallback(Ref* pSender, Vec2 pos);

	/**
	 * 进入游戏用户首次答对题目，提示的子弹没有撒网捕鱼
	 */
	void setTip(bool tip);

private:

	bool m_tip = false;

};

//*************************************************************** 捕鱼效果的类 GetFish ********************************************************//

class GetFish: public Sprite {
public:
	static GetFish* createWithSpriteFrame(SpriteFrame *spriteFrame);

	GetFish();
	virtual ~GetFish();

	void runActionWithAnimation();

	void actionEndCallback(Ref* pSender);

private:

};

//******************************************************** 捕鱼的网的类 Net ********************************************************//

class Net: public Sprite {
public:
	static Net* createWithSpriteFrame(SpriteFrame *spriteFrame);

	Net();
	virtual ~Net();

	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	bool isInView(Vec2 pos);
	void runActionWithAnimation();
	void runActionTipAnimation();
	void actionEndCallback(Node* pSender);
	void callbackRemoveByself(Node* pSender);

private:

};

//************************************************************* 提示手势的类 Hand ********************************************************//

class Hand: public Sprite {
public:
	const static int HAND_MOVE_ANSER = 0;
	const static int HAND_CATVH_FISH = 1;
	static Hand* createWithSpriteFrame(SpriteFrame *spriteFrame);

	Hand();
	virtual ~Hand();

	void runActionTipFish(Vec2 to);
	void runActionWithAnimation(Vec2 to);

	void actionTipNetCallback(Ref* pSender);
	void actionTipFishEndCallback(Ref* pSender);
	void actionNotTouchCallback(Ref* pSender);
	void actionEndCallback(Ref* pSender);
	void removeSelfCallback(Ref* pSender);

    virtual void setPosition(const Vec2 &position);

private:
	Vec2 m_orignal_vec2;
	int m_tip_fish = 0;
};

//********************************************************************Toast***********************************************************//
class Toast: public LayerColor {
public:
    static Toast* create(const std::string& text, float showdt);

    Toast();
	virtual ~Toast();

    bool init(const std::string& text, int dt);


private:
	void end(float dt);

};

//************************************************************* 原点到目标点，对象要转动的角度 MyRotate ******************************************************//

class MyRotate: public Ref {
public:

	static float rotate(Vec2 dstvec, Vec2 orivec);

};


//********************************************************** 把对应的数字转换成字母 MyString ***********************************************************//

class MyString: public Ref {
public:

	static std::string getLeffter(int which);

	static std::string getString(int which);
};
//************************************************************ 产生随机数的类  MyRandom **************************************************************//
class MyRandom : public Ref {
public :

	// 随机获得 start ~ end 之间的一个数字
	static int randomNumber(int start,int end);

	// 获得 0 ~ totalnumber-1 的乱序数组
//	static __Array* randomOrder(int total_number);

	// 获得 0 ~ totalnumber-1 的乱序数组
	static int* randomOrder(int totalnumber);

	static Vector<__Integer*> randomVector(int totalnumber);
};


//************************************************************* 对图片读取的包装 ReadResources ***********************************************************//
class ReadResources : public Ref {
public:

	/**
	 * @param name: 图片的名称，去除后标
	 * @param start: 第几张开始读取图片
	 * @param length: 读取几张图片
	 * @param unitdelay: 每张图片刷新的间隔，单位是毫秒
	 */
	static Animation* animation(const char* name, int start, int length, float unitdelay);

	/**
	 * @param name: 图片的名称，去除后标
	 * @param length: 读取几张图片
	 * @param unitdelay: 每张图片刷新的间隔，单位是毫秒
	 */
	static Animation* animation(const char* name, int length, float unitdelay);

	/**
	 * @param name: 图片的名称，去除后标
	 * @param length: 读取几张图片
	 * @param saveTag: 这组动画的缓存标志，供下次直接读取
	 */
	static Animation* animation(const char* name, int length, const char* saveTag);

	/**
	 * @param name: 图片的名称，去除后标
	 * @param length: 读取几张图片
	 * @param unitdelay: 每张图片刷新的间隔
	 * @param saveTag: 这组动画的缓存标志，供下次直接读取
	 */
	static Animation* animation(const char* name, int length, float unitdelay, const char* saveTag);

	/**
	 * @param name: 图片的名称，去除后标
	 * @param start: 第几张开始读取图片
	 * @param length: 读取几张图片
	 * @param saveTag: 这组动画的缓存标志，供下次直接读取
	 */
	static Animation* animation(const char* name, int start, int length, const char* saveTag);

	/**
	 * @param name: 图片的名称，去除后标
	 * @param start: 第几张开始读取图片
	 * @param length: 读取几张图片
	 * @param unitdelay: 每张图片刷新的间隔
	 * @param saveTag: 这组动画的缓存标志，供下次直接读取
	 */
	static Animation* animation(const char* name, int start, int length, float unitdelay, const char* saveTag);

//	static Animation* animation(const char* name, int length, const std::string& saveTag);
//	static Animation* animation(const char* name, int length, float unitdelay, const std::string& saveTag);
//	static Animation* animation(const char* name, int start, int length, const std::string& saveTag);
//	static Animation* animation(const char* name, int start, int length, float unitdelay, const std::string& saveTag);
};

#endif /* GAMEELEMENT_H_ */
