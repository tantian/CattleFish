/*
 * GameScene.h
 *
 *  Created on: 2015-11-26
 *      Author: dhm
 */

#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include "cocos2d.h"
#include "GameElement.h"
#include "JniToJava.h"
#include "vector"
#include "ReadJSON.h"

USING_NS_CC;

//namespace cattlefish {

/**
@class GameScene
@brief Base class for cocos2d::Layer.
@details the game interface of the CattleFish
*/
class GameScene: public cocos2d::Layer {
public:
	GameScene();
	virtual ~GameScene();

	static cocos2d::Scene* createScene();

    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);

    /**
     * 第一次进入这个会会跑这个函数
     */
    virtual void onEnter();

    /**
     * 退出这个类时，会运行这个函数
     */
    virtual void onExit();

    /**
     * 捕获机器物理按键
     */
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);


	virtual bool init();

	void loadResource(float dt);
	void secondLoadRes(float dt);

	void menuCloseCallback(cocos2d::Ref* pSender);
	void menuAgainCallback(cocos2d::Ref* pSender);
	void menuSoundCallback(cocos2d::Ref* pSender);

	/**
	 * 选项移到炮架的回调函数
	 */
	void optionMovetoBogyCallback(Node* pSender);

	/**
	 * 创建鱼
	 */
	void createFish(float dt);

	/**
	 * 增加分数
	 */
	void addScore();

	/**
	 * 创建效果鱼
	 */
	void createGetFish(Vec2 pos);

	/**
	 * 提示捕鱼子弹运行的位置
	 */
	void tipFiredBullet(Vec2 touchlac);

	/**
	 * 捕鱼子弹运行位置
	 */
	void firedBullet(Vec2 touchlac);

	/**
	 * 答对题目，处理东西
	 */
	void doRight();

	/**
	 * 答错题目，处理东西
	 */
	void doWrong();

	/**
	 * 失去生命值
	 */
	void removeLife(cocos2d::Ref* pSender);

	/**
	 * 初始化一道题
	 */
	void initQuestion();

	/**
	 * 每答对一道题，用户捕鱼次数的增加
	 */
	void addCatchFishTimes();

	/**
	 * 没有捕到鱼
	 */
	void notCatchFish();

	/**
	 *
	 */
	Question* getQuestion(int which);

	int getDone() {return m_done;};

    CREATE_FUNC(GameScene);

private:

    MenuItemImage* getSoundItem();

    /**
     * 用户选择答案
     */
    void userChoiceLeffter(cocos2d::Touch* touch, cocos2d::Event* event);

    void timeCounter(float dt);

    /**
     * 第一次进入游戏。初始化题目
     */
	void initFirstQuestion();

    void reset();

    /**
     * 用户选择答案之后，答案，题目框显示灰色，不可点击状态
     */
    void hadGraySprite(const Color3B& color);

    /**
     * 游戏结束显示结束界面
     */
    void showEndLayer(bool isSuc, int done);

    void showSprite(cocos2d::Ref * pSender);

    /***************************************** 游戏界面显示层的标志 begin ***********************************************/
    const static int Z_WATER = 0;
    const static int Z_FISH = 1;
    const static int Z_WATER_EFFECT = 2;
    const static int Z_GRASS = 4;
    const static int Z_CATTLE = 6;
    const static int Z_QST = 7;
    const static int Z_LAND_PACKER = 8;
    const static int Z_CLOSE_BTN = 16;
    /***************************************** 游戏界面显示层的标志 end ***********************************************/

//    const static char[3][100] BOGY = {"bogy_normal", "bogy_bomb", "bogy_launch"};
    /**
     * 做过题目数
     */
    int m_done = 0;

    /**
     * 游戏获得分数
     */
    int m_score = 0;

    /**
     * 游戏时间
     **/
    int m_time = 0;

    /**
     *
     */
    int m_new_fish_counter = 0;

    /**
     * 每道答对题目，捕鱼的次数
     */
    int m_catch_fish_counter = 0;

    /**
     * 当前游戏要做完多少题，才结束
     */
    int m_total_title_number = 0;

    // 创建捕到鱼效果 成功
    bool m_had_new_get_fish = false;

    /**
     * 是否显示游戏结束界面
     */
    bool m_show_end = false;

    /**
     * 第一阶段加载资源的标志
     */
    bool m_first_load = false;

    /**
     * 第二阶段加载资源的标志
     */
    bool m_second_load = false;

    /**
     * 是否要提示捕鱼
     */
    int m_tip_fish = 0;

    bool m_data_json = false;

    /**
     * 用户选择答案的id
     */
    int m_choice_leffter = -1;

//    ReadJSON* m_read_json;

    /**
     * 鱼层
     */
    SpriteBatchNode* m_fishbath;

    /**
     * 地面上层
     */
    SpriteBatchNode* m_landbath;

    /**
     * 水波纹层
     */
    SpriteBatchNode* m_watereffectbath;

    /**
     * 捕鱼效果层
     */
    SpriteBatchNode* m_getfishbath;

    /**
     * 游戏界面显示题目标签的引用
     */
    QuestionLabel* m_qstlabel;

    /**
     * 牛的引用
     */
    Cattle* m_cattle;

    /**
     * 炮架的引用
     */
    Bogy* m_bogy;

    /**
     * 每道题捕鱼计数
     */
    Label* m_counter_label;

    /**
     * 游戏界面触屏监听器
     */
    EventListenerTouchOneByOne* m_touchone_lstr;
    EventListenerKeyboard* m_key_lstnr;

    /**
     * A、B、C字母列表
     */
    __Array* m_letterlist;

    /**
     * 牛生命值头像
     */
    __Array* m_cattlehead;

    /**
     * 做过题目保存
     */
    __Array* m_qstlist;

    /**
     * 题目顺序
     */
    int* m_qstOrderlst;

    /**
     * 做错题目题号
     */
    std::vector<int> m_err_qst_num;

};

//} /* namespace cattlefish */
#endif /* GAMESCENE_H_ */
