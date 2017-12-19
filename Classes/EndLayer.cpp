/*
 * EndLayer.cpp
 *
 *  Created on: 2015-9-1
 *      Author: guh
 */

#include "EndLayer.h"
#include "GameElement.h"
#include "GameScene.h"

#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

EndLayer::EndLayer() {
	// TODO Auto-generated constructor stub
	log("EndLayer EndLayer() !!! ");
}

EndLayer::~EndLayer() {
	// TODO Auto-generated destructor stub
	log("EndLayer ~EndLayer() !!! ");
}


EndLayer* EndLayer::create(bool isSuc) {
	auto layer = (EndLayer*)new (std::nothrow) EndLayer();
	layer->m_suc = isSuc;
	 if (layer && layer->init())
	 {
		layer->autorelease();
	 }
	return layer;
}

void EndLayer::isSuc(bool isSuc) {
	m_suc = isSuc;
}

bool EndLayer::init() {
    if ( !Layer::init() )
    {
        return false;
    }


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto color = LayerColor::create(Color4B(0, 0, 0, 121));
	this->addChild(color);
	auto closeItem = MenuItemImage::create(
                                       "exit00.png",
                                       "exit01.png",
                                       CC_CALLBACK_1(EndLayer::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width/2 + closeItem->getContentSize().width ,
                            origin.y + visibleSize.height*0.07f));

	auto againItem = MenuItemImage::create(
                                       "again00.png",
                                       "again01.png",
                                       CC_CALLBACK_1(EndLayer::menuAgainCallback, this));

	againItem->setPosition(Vec2(origin.x + visibleSize.width/2 - againItem->getContentSize().width ,
                            origin.y + visibleSize.height*0.07f));

	m_pre_btn = MenuItemImage::create(
                                       "pre00.png",
                                       "pre01.png",
                                       CC_CALLBACK_1(EndLayer::menuPreCallback, this));

	m_pre_btn->setPosition(Vec2(origin.x +visibleSize.width/2 - m_pre_btn->getContentSize().width*2,
                            origin.y + visibleSize.height*0.225f));

	m_next_btn = MenuItemImage::create(
                                       "next00.png",
                                       "next01.png",
                                       CC_CALLBACK_1(EndLayer::menuNextCallback, this));

	m_next_btn->setPosition(Vec2(origin.x + visibleSize.width/2 + m_next_btn->getContentSize().width*2,
                            origin.y + visibleSize.height*0.225f));

	auto menu = Menu::create(closeItem, againItem, m_pre_btn, m_next_btn, NULL);
	menu->setPosition(Vec2::ZERO);
	loadResource(0);
	this->addChild(menu, 8);

	/****************************************** 游戏结束界面弹出来的时，屏蔽游戏界面的触摸效果    begin ************************************************/
	auto callback = [](Touch * ,Event *)     {
		return true;
	};
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = callback;
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	/****************************************** 游戏结束界面弹出来的时，屏蔽游戏界面的触摸效果    end ************************************************/

	return true;
}

void EndLayer::loadResource(float dt) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto m_endbath = (SpriteBatchNode*)SpriteBatchNode::create("end.png");
	this->addChild(m_endbath, 0);
	auto m_cache=SpriteFrameCache::getInstance();
	m_cache->addSpriteFramesWithFile("end.plist");

	auto frame = m_cache->getSpriteFrameByName("dakuang.png");
	auto sprite = Sprite::createWithSpriteFrame(frame);
	m_bg_sz = sprite->getContentSize();
	m_bg_vec2 = Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2+m_bg_sz.height*0.08f);
	sprite->setPosition(m_bg_vec2);
	m_endbath->addChild(sprite);

	char szNm[100]="cattle_fail";
	auto cattleFrame = m_cache->getSpriteFrameByName("cattle_fail00.png");
	int cattleL = 10;
	auto audio = SimpleAudioEngine::getInstance();
	if (m_suc) {
		audio->playEffect("snd/snd_suc.ogg", false, 1.0f, 0, 2.0f);
		frame = m_cache->getSpriteFrameByName("chengon.png");
		sprintf(szNm, "cattle_suc");
		cattleL = 15;
	} else {
		audio->playEffect("snd/snd_fail.ogg", false, 1.0f, 0, 2.0f);
		frame = m_cache->getSpriteFrameByName("shibai.png");
		cattleFrame = m_cache->getSpriteFrameByName("cattle_suc00.png");
	}

	auto sucSprite = Sprite::createWithSpriteFrame(frame);
	sucSprite->setPosition(m_bg_vec2 + Vec2(m_bg_sz.width/2-sucSprite->getContentSize().width, m_bg_sz.height/2-sucSprite->getContentSize().height));
	m_endbath->addChild(sucSprite);

	auto cattleSprite = Cattle::createWithSpriteFrame(cattleFrame);
	cattleSprite->setPosition(m_bg_vec2 + Vec2(cattleSprite->getContentSize().width-m_bg_sz.width/2, m_bg_sz.height/2-cattleSprite->getContentSize().height*0.6f));
	m_endbath->addChild(cattleSprite);

	auto anim = ReadResources::animation(szNm, cattleL, 0.1);
	cattleSprite->runActionWithAnimation(anim, Cattle::FAIL);

	frame = m_cache->getSpriteFrameByName("cuo.png");
	m_RightOrWrong = Sprite::createWithSpriteFrame(frame);
//	frame = m_cache->getSpriteFrameByName("dui.png");
	m_RightOrWrong->setPosition(m_bg_vec2 + Vec2(m_bg_sz.width/4, 0 - m_bg_sz.height*0.3f));
	m_endbath->addChild(m_RightOrWrong);
	if (visibleSize.width < visibleSize.height) {
		sucSprite->setPosition(m_bg_vec2 + Vec2(m_bg_sz.width/2-sucSprite->getContentSize().width*0.66f, m_bg_sz.height/2-sucSprite->getContentSize().height));
		cattleSprite->setPosition(m_bg_vec2 + Vec2(cattleSprite->getContentSize().width*0.66f-m_bg_sz.width/2, m_bg_sz.height/2-cattleSprite->getContentSize().height*0.6f));
//		m_RightOrWrong->setPosition(m_bg_vec2 + Vec2(m_bg_sz.width/4, 0 - m_bg_sz.height*0.3f));
	}
}

void EndLayer::setQuestion(__Array* qstlist, int done) {
	m_Done = done;
	int right = 0;
	for (int i=0; i<m_Done; i++) {
		log("set Question i is %d ", i);
		auto qst = (Question*)qstlist->getObjectAtIndex(i);
		m_qstlist.pushBack(qst);
		if (qst->getRightId() == qst->getChoice()) {
			right++;
		}
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float factor = Director::getInstance()->getContentScaleFactor();
	int fontSize = 24/factor;

	auto m_cache=SpriteFrameCache::getInstance();

	auto qst = m_qstlist.at(m_curshow);
//	if (visibleSize.width > visibleSize.height) {
		m_qstlabel = QuestionLabel::createWithSystemFont(qst, "fonts/Readboy-Code.ttf", fontSize, Size(m_bg_sz.width*0.8, m_bg_sz.height/2));
//	} else {
//		m_qstlabel = QuestionLabel::createWithSystemFont(qst, "fonts/readboy_code.ttf", fontSize, Size(m_bg_sz.width*0.8, m_bg_sz.height/2));
//	}
//	m_qstlabel->setDimensions(m_bg_sz.width*0.8, m_bg_sz.height/2);
	m_qstlabel->setVerticalAlignment(TextVAlignment::CENTER);
	m_qstlabel->setSpaceLine(true);
	m_qstlabel->setQuestion(1, qst);
	this->addChild(m_qstlabel, 1);
	m_qstlabel->setPosition(m_bg_vec2);

    std::string rightTitle = "答对 : ";
	auto rightLabel = Label::createWithSystemFont(rightTitle.append(MyString::getString(right)), "fonts/arial.ttf", 30/factor);
	this->addChild(rightLabel, 2);
	rightLabel->setPosition(m_bg_vec2 + Vec2(m_bg_sz.width/4, m_bg_sz.height/4));

    std::string curshow = MyString::getString(m_curshow+1);
    curshow.append("/");
    curshow.append(MyString::getString(m_Done));
	m_CurShowLabel = Label::createWithSystemFont(curshow, "fonts/arial.ttf", fontSize);
	this->addChild(m_CurShowLabel, 3);
	m_CurShowLabel->setPosition(m_bg_vec2 + Vec2(0, 0-m_bg_sz.height*0.42f));
	m_pre_btn->setPositionY(m_CurShowLabel->getPositionY());
	m_next_btn->setPositionY(m_CurShowLabel->getPositionY());

    std::string myansr = "我的答案: ";
    std::string rightAnsr = "正确答案: ";
    float param_right_cho_h = 0.3f;
    Size right_cho_size = Size(m_bg_sz.width*0.19f, fontSize);
    float param_cho_l = 0.13f;
    float param_right_l = 0.2f;
    if (visibleSize.width < visibleSize.height) {
    	right_cho_size = Size(m_bg_sz.width*0.27f, fontSize);
    	param_right_cho_h = 0.28f;
    	rightLabel->setPosition(m_bg_vec2 + Vec2(m_bg_sz.width/4, m_bg_sz.height*0.22f));
    	m_RightOrWrong->setPosition(m_bg_vec2 + Vec2(right_cho_size.width*1.45f, 0 - m_bg_sz.height*param_right_cho_h));
        param_cho_l = 0.21f;
        param_right_l = 0.27f;
    }
    m_choiceAnsrLabel = Label::createWithSystemFont(myansr.append(MyString::getLeffter(qst->getChoice())), "fonts/arial.ttf", fontSize, right_cho_size);
    m_RightAnsrLabel = Label::createWithSystemFont(rightAnsr.append(MyString::getLeffter(qst->getRightId())), "fonts/arial.ttf", fontSize, right_cho_size);
//	m_choiceAnsrLabel->setPosition(m_bg_vec2 + Vec2(m_bg_sz.width*0.13f, 0-m_bg_sz.height*0.305f));
//	m_RightAnsrLabel->setPosition(m_bg_vec2 + Vec2(0-m_bg_sz.width*0.2f, 0-m_bg_sz.height*0.305f));
    m_choiceAnsrLabel->setPosition(m_bg_vec2 + Vec2(m_bg_sz.width*param_cho_l, 0-m_bg_sz.height*(param_right_cho_h+0.005f)));
    m_RightAnsrLabel->setPosition(m_bg_vec2 + Vec2(0-m_bg_sz.width*param_right_l, 0-m_bg_sz.height*(param_right_cho_h+0.005f)));
	this->addChild(m_choiceAnsrLabel, 4);

	this->addChild(m_RightAnsrLabel, 5);

	auto frame = m_cache->getSpriteFrameByName("cuo.png");
	if (qst->getRightId() == qst->getChoice()) {
		frame = m_cache->getSpriteFrameByName("dui.png");
	}
	m_RightOrWrong->setSpriteFrame(frame);

//	JniToJava::savePlanData(right, done, 0, 0);
}

void EndLayer::refresh() {
	auto qst = m_qstlist.at(m_curshow);
	m_qstlabel->setQuestion(m_curshow+1, qst);

    std::string curshow = MyString::getString(m_curshow+1);
    curshow.append("/");
    curshow.append(MyString::getString(m_Done));
    m_CurShowLabel->setString(curshow);

    std::string myansr = "我的答案: ";
    m_choiceAnsrLabel->setString(myansr.append(MyString::getLeffter(qst->getChoice())));

    std::string rightAnsr = "正确答案: ";
    m_RightAnsrLabel->setString(rightAnsr.append(MyString::getLeffter(qst->getRightId())));

	auto m_cache = SpriteFrameCache::getInstance();
	auto frame = m_cache->getSpriteFrameByName("cuo.png");
	if (qst->getRightId() == qst->getChoice()) {
		frame = m_cache->getSpriteFrameByName("dui.png");
	}
	m_RightOrWrong->setSpriteFrame(frame);
}

void EndLayer::menuAgainCallback(cocos2d::Ref* pSender) {
	this->getParent()->removeChild(this, true);

	auto director = Director::getInstance();
	log("running scene ");
	auto gamescene = (GameScene*)director->getRunningScene()->getChildByName("layer_game");
	gamescene->menuAgainCallback(pSender);
	log(" scene after ");

}

void EndLayer::menuPreCallback(cocos2d::Ref* pSender) {
	m_curshow--;
	if (m_curshow < 0) {
		m_curshow = m_Done-1;
	}
	refresh();
}

void EndLayer::menuNextCallback(cocos2d::Ref* pSender) {
	m_curshow++;
	if (m_curshow > m_Done-1) {
		m_curshow = 0;
	}
	refresh();
}

void EndLayer::menuCloseCallback(cocos2d::Ref* pSender) {

	JniToJava* jniptr = JniToJava::create();
		jniptr->release();
		jniptr->releaseRe();

	    Director::getInstance()->end();
   // Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
