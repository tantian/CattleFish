/*
 * EndLayer.h
 *
 *  Created on: 2015-9-1
 *      Author: guh
 *  勇敢牛去捕鱼的UI界面
 *
 *
 *
 */

#ifndef ENDLAYER_H_
#define ENDLAYER_H_

#include "cocos2d.h"
#include "GameElement.h"
#include "Question.h"

USING_NS_CC;

/**
@class EndLayer
@brief Layer.
@details 游戏结束界面;
*/
class EndLayer: public cocos2d::Layer {
public:

	/**
	 * 析构函数
	 */
	virtual ~EndLayer();

	/**
	 * 静态创建一个结束界面
	 */
	static EndLayer* create(bool isSuc);

	/**
	 * 设置游戏是否成功
	 */
	void isSuc(bool isSuc);

	virtual bool init();

	/**
	 * 结束界面加载资源
	 */
	void loadResource(float dt);

	/**
	 * 结束界面，关闭按钮的回调函数
	 */
	void menuCloseCallback(cocos2d::Ref* pSender);

	/**
	 * 结束界面，再来一遍按钮的回调函数
	 */
	void menuAgainCallback(cocos2d::Ref* pSender);

	/**
	 * 结束界面，上一道题按钮的回调函数
	 */
	void menuPreCallback(cocos2d::Ref* pSender);

	/**
	 * 结束界面，下一道按钮的回调函数
	 */
	void menuNextCallback(cocos2d::Ref* pSender);

	/**  设置结束界面，要显示的题目
	 * 	 qstlist: 显示的题目列表
	 * 	 done 做过的题目数，也就是qstlist的包含题目个数
	 * */
	void setQuestion(__Array* qstlist, int done);

	/**
	 * 刷新结束界面
	 */
	void refresh();

    CREATE_FUNC(EndLayer);

private:
    /**
     * 构造函数
     */
	EndLayer();

	/**
	 * 做了几道题
	 */
	int m_Done = 0;

	/**
	 * 当前显示第几道题
	 */
	int m_curshow = 0;

	const float m_ans_y_rate = 0.273f;

	bool m_suc = false;

	/**
	 * 结束界面背景框的位置
	 */
	Vec2 m_bg_vec2 = Vec2::ZERO;

	/**
	 * 结束界面背景框的大小
	 */
	Size m_bg_sz = Size::ZERO;

	/**
	 * 上一道题的按钮引用
	 */
	MenuItemImage* m_pre_btn;

	/**
	 * 下一道题按钮的引用
	 */
	MenuItemImage* m_next_btn;

	/**
	 * 显示题目的标签
	 */
	QuestionLabel* m_qstlabel;

	/**
	 * 存放做过题目的列表
	 */
	Vector<Question*> m_qstlist;

	/**
	 * 显示正确答案的标签
	 */
	Label* m_RightAnsrLabel;

	/**
	 * 显示用户选择答案的标签
	 */
	Label* m_choiceAnsrLabel;

	/**
	 * 显示当前第几道题的标签
	 */
	Label* m_CurShowLabel;

	/**
	 * 显示用户答对答错
	 */
	Sprite* m_RightOrWrong;
};

#endif /* ENDLAYER_H_ */
