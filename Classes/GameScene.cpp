/*
 * GameScene.cpp
 *
 *  Created on: 2015-11-26
 *      Author: guh
 */

#include "GameScene.h"
#include "EndLayer.h"
#include "Fish.h"
#include "UVSprite.h"

#include "SimpleAudioEngine.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "ReadJSON.h"

#include "cocostudio/CocoStudio.h"

USING_NS_CC_EXT;

using namespace cocostudio;
using namespace CocosDenshion;
using namespace ui;
extern "C" {
JNIEXPORT void JNICALL Java_org_cocos2dx_lib_CocosBroadcastReceiver_nativeUnseen(JNIEnv* env, jobject thiz) {
	cocos2d::Director::getInstance()->getRunningScene()->setVisible(false);
	log("MainActivity unseen");
}}
//namespace cattlefish {

GameScene::GameScene()
: m_done(0),
  m_score(0),
  m_new_fish_counter(0),
  m_catch_fish_counter(0),
  m_total_title_number(0),
  m_had_new_get_fish(false),
  m_show_end(false),
  m_first_load(false),
  m_second_load(false),
  m_tip_fish(0),
  m_choice_leffter(-1),
  m_fishbath(nullptr),
  m_watereffectbath(nullptr),
  m_getfishbath(nullptr),
  m_landbath(nullptr),
  m_qstlabel(nullptr),
  m_cattle(nullptr),
  m_bogy(nullptr),
  m_counter_label(nullptr),
  m_letterlist(nullptr),
  m_cattlehead(nullptr),
  m_qstlist(nullptr),
  m_qstOrderlst(nullptr),
  m_touchone_lstr(nullptr),
  m_key_lstnr(nullptr)
 {
	m_letterlist = __Array::createWithCapacity(3);
	m_letterlist->retain();

	m_cattlehead = __Array::createWithCapacity(3);
	m_cattlehead->retain();

	m_qstlist = __Array::createWithCapacity(10);
	m_qstlist->retain();

	log("GameScene GameScene() !!! ");
}

GameScene::~GameScene() {
	m_letterlist->removeAllObjects();
	CC_SAFE_RELEASE_NULL(m_letterlist);

	m_cattlehead->removeAllObjects();
	CC_SAFE_RELEASE_NULL(m_cattlehead);

	m_qstlist->removeAllObjects();
	CC_SAFE_RELEASE_NULL(m_qstlist);

	log("GameScene ~GameScene() !!! ");
	auto m_animcache = AnimationCache::getInstance();
	m_animcache->destroyInstance();
	auto m_cache=SpriteFrameCache::getInstance();
	m_cache->removeUnusedSpriteFrames();
	auto m_txturecache = Director::getInstance()->getTextureCache();
	m_txturecache->removeAllTextures();

	auto ss = m_txturecache->getCachedTextureInfo();
	log("~GameScene() texturecache %s ", ss.c_str());
}

cocos2d::Scene* GameScene::createScene() {
	//cocos2d::Director::getInstance()->getRunningScene()->

    auto scene = Scene::create();

    auto layer = GameScene::create();

    scene->addChild(layer, 0, "layer_game");

    return scene;
}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	if (m_first_load) {
		auto m_hand = m_landbath->getChildByName("sprite_hand");
		if (m_hand) {
			m_landbath->removeChild(m_hand, true);
			if (m_tip_fish >= 2) {
				m_counter_label->setString("3");
				m_counter_label->setRotation(0.0f);
			}
		}
		auto qst = (Question*)m_qstlist->getObjectAtIndex(m_done);
		if (qst->getDone() && qst->getRightId()==qst->getChoice() && m_catch_fish_counter<3) {
			Vec2 touchlac = touch->getLocation();
			Size visibleSize = Director::getInstance()->getVisibleSize();
			if (touchlac.y > visibleSize.height/2) {
				firedBullet(touchlac);
			}
		}
		if (m_choice_leffter < 0) {
			userChoiceLeffter(touch, event);
		}
	}
	return true;
}

void GameScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {
	if (m_first_load) {
		if (m_choice_leffter < 0) {
			userChoiceLeffter(touch, event);
		} else {
			auto letter = (Leffter*)m_letterlist->getObjectAtIndex(m_choice_leffter);
			if (letter) {
				letter->onTouchMoved(touch, event);
			}
		}
	}
}

void GameScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
	if (m_choice_leffter >=0) {
		auto letter = (Leffter*)m_letterlist->getObjectAtIndex(m_choice_leffter);
		if (letter) {
			if (m_bogy->isInView(letter->getPosition())) {
				optionMovetoBogyCallback(this);
			} else {
				if (!letter->isAutoMove()) {
					bool isDown = letter->isTouchDowned(touch->getLocation());
					if (isDown && !letter->isMove()) {
						letter->runActionWithAnimation(m_bogy->getPosition(),
								CallFuncN::create(CC_CALLBACK_1(GameScene::optionMovetoBogyCallback, this)));
					} else {
						letter->reset();
						m_choice_leffter = -1;
					}
				}
			}
		}
	}
}

void GameScene::onTouchCancelled(Touch *touch, Event *unused_event) {
	log("GameScene onTouchCancelled() !!! ");
	if (m_choice_leffter >= 0) {
		auto letter = (Leffter*)m_letterlist->getObjectAtIndex(m_choice_leffter);
		if (letter) {
			letter->reset();
		}
	}
	m_choice_leffter = -1;
}

void GameScene::onEnter() {
	Layer::onEnter();
	log("GameScene onEnter() !!! ");

//	auto userdef = UserDefault::getInstance();
//	m_tip_fish = userdef->getIntegerForKey("cattle_fish_tip_fish", 0);
	if (!m_touchone_lstr) {
		m_touchone_lstr = EventListenerTouchOneByOne::create();
		m_touchone_lstr->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
		m_touchone_lstr->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
		m_touchone_lstr->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
		m_touchone_lstr->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled, this);

		_eventDispatcher->addEventListenerWithSceneGraphPriority(m_touchone_lstr, this);
	}

	if (!m_key_lstnr) {
		m_key_lstnr = EventListenerKeyboard::create();
		m_key_lstnr->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_key_lstnr,this);
	}
	this->schedule(schedule_selector(GameScene::createFish), 2.0f);
	this->schedule(schedule_selector(GameScene::timeCounter), 1.0f);
}

void GameScene::onExit() {
	this->unscheduleAllCallbacks();
	log("GameScene onExit() !!! ");
//	auto userdef = UserDefault::getInstance();
//	userdef->setIntegerForKey("cattle_fish_tip_fish", m_tip_fish);
//	userdef->flush();

	Layer::onExit();
	//游戏退出时，关闭背景音乐
	//SimpleAudioEngine* audio = SimpleAudioEngine::getInstance();
	//audio->stopAllEffects();
	//audio->stopBackgroundMusic(true);
	//log("music stop:");
	//add by wen
//	audio->end();
	if (m_touchone_lstr) {
		_eventDispatcher->removeEventListener(m_touchone_lstr);
	}
	if (m_key_lstnr) {
		Director::getInstance()->getEventDispatcher()->removeEventListener(m_key_lstnr);
	}
//	this->unschedule(schedule_selector(GameScene::createFish));
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK || keyCode == EventKeyboard::KeyCode::KEY_HOME) {
		Director::getInstance()->end();
	}
}

bool GameScene::init() {
    if (!Layer::init())
    {
        return false;
    }
	log("GameScene init() !!! ");
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto watersprite = Sprite::create("humian.png");
	watersprite->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height-watersprite->getContentSize().height/2));
	this->addChild(watersprite, Z_WATER, "water_bg");

	auto grassprite = Sprite::create("caodi.png");
	grassprite->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+grassprite->getContentSize().height/2));
	this->addChild(grassprite, Z_GRASS, "sprite_grass");

//	log("vis w is %f, h is %f, x is %f, y is %f ", visibleSize.width, visibleSize.height, origin.x, origin.y);
	auto closeItem = MenuItemImage::create("close00.png", "close01.png", CC_CALLBACK_1(GameScene::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width*2/3 ,
                            origin.y + visibleSize.height-closeItem->getContentSize().height*2/3));

	MenuItemImage* soundItem = MenuItemImage::create("horn_normal.png", "horn_click.png", CC_CALLBACK_1(GameScene::menuSoundCallback, this));
	soundItem->setPosition(origin + Vec2(visibleSize.width*0.80f , visibleSize.height*0.15f));
	soundItem->setName("soundItem");
	soundItem->setVisible(false);

	auto menu = Menu::create(closeItem, soundItem, NULL);
	menu->setName("button_menu");
	menu->setPosition(Vec2::ZERO);

	this->addChild(menu, Z_CLOSE_BTN);

    this->scheduleOnce(schedule_selector(GameScene::loadResource), 0.04f);
	return true;
}

void GameScene::timeCounter(float dt) {
	if (m_show_end) {
		return;
	}
	m_time++;
}

void GameScene::createFish(float dt) {
	if (m_show_end) {
		return;
	}
	m_new_fish_counter++;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	if (m_fishbath) {
		int fish_number = m_fishbath->getChildrenCount();
		bool create = false;
		if (fish_number < 3) {
			create = true;
		} else if (fish_number < 6) {
			if (m_new_fish_counter > 5) {
				create = true;
			}
		} else if (fish_number<9) {
			if (m_new_fish_counter > 10) {
				create = true;
			}
		} else if (fish_number<12) {
			if (m_new_fish_counter > 20) {
				create = true;
			}
		} else if (fish_number<15) {
			if (m_new_fish_counter > 40) {
				create = true;
			}
		} else if (fish_number<18) {
			if (m_new_fish_counter > 80) {
				create = true;
			}
		}
		if (create) {
			auto m_cache=SpriteFrameCache::getInstance();
			auto frame = m_cache->getSpriteFrameByName("fish_one00.png");
			float type = rand_0_1();
			int fishtype = type*4;
			log("type is %f, fishtype is %d", type, fishtype);
			auto sprite = Fish::createWithSpriteFrame(frame, fishtype);
			float rand = rand_0_1();
			int addh = rand*visibleSize.height/2;
			sprite->setPosition(Vec2(origin.x, origin.y+visibleSize.height/2+addh));
			m_fishbath->addChild(sprite);
			sprite->setFishRoute(Fish::INTERVALDT);
			m_new_fish_counter = 0;
		}
	}
}

void GameScene::firedBullet(Vec2 touchlac) {
	float degree = MyRotate::rotate(touchlac, m_bogy->getPosition());
	m_bogy->setRotation(degree);
	m_bogy->runActionWithAnimation(touchlac, Bogy::LAUNCH);
	m_had_new_get_fish = false;
	m_catch_fish_counter++;
    __String* counters = CCString::createWithFormat("%d", (3-m_catch_fish_counter));
	m_counter_label->setString(counters->_string);
	m_counter_label->setRotation(degree);

	auto audio = SimpleAudioEngine::getInstance();
	audio->playEffect("snd/snd_fired.ogg", false, 1.0f, 0, 2.0f);
}

void GameScene::tipFiredBullet(Vec2 touchlac) {
	float degree = MyRotate::rotate(touchlac, m_bogy->getPosition());
	m_bogy->setRotation(degree);
	m_bogy->runActionTip(touchlac);
	m_counter_label->setRotation(degree);

}

void GameScene::doRight() {
	m_cattle->runActionWithAnimation(Cattle::HAPPY);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto m_cache = SpriteFrameCache::getInstance();
	if (m_tip_fish < 2) {
		auto frame = m_cache->getSpriteFrameByName("hand00.png");
		auto m_hand = Hand::createWithSpriteFrame(frame);
		m_hand->setPosition(m_bogy->getPosition());
		m_hand->runActionTipFish(Vec2(origin.x+visibleSize.width/4, origin.y+visibleSize.height*2/3));
		m_landbath->addChild(m_hand, 4, "sprite_hand");
		m_tip_fish ++;
	}

//	fPitch: [0~n] 值越大音调越高，同时播放时间越短，可以调出高八度低八度之类的，不调整是1
//	fPan: [-1~1] 左右声道控制，0是双声道
//	fGain: [0~n] 音量大小， 1是不调整
    __String* counters = CCString::createWithFormat("%d", (3-m_catch_fish_counter));
	m_counter_label->setString(counters->_string);
	m_counter_label->setVisible(true);
	auto audio = SimpleAudioEngine::getInstance();
	audio->playEffect("snd/snd_do_right.ogg", false, 1.0f, 0, 2.0f);
	auto stat= ParticleSystemQuad::create("particle/sucstat.plist");
	stat->setPosition(m_cattle->getPosition()+Vec2(0, 50));
	this->addChild(stat, Z_CATTLE);
}

void GameScene::doWrong() {
	m_cattle->runActionWithAnimation(Cattle::ANGRY);
	auto cattlehead = (Sprite*)m_cattlehead->getLastObject();
	auto aciton = FadeOut::create(2.0f);
	auto seq = (ActionInterval*)CCSequence::create(aciton,
		CallFuncN::create(CC_CALLBACK_1(GameScene::removeLife, this)), NULL);
	auto repeat = CCRepeat::create(seq, 1);
	cattlehead->runAction(repeat);
	m_bogy->setColor(Color3B(144, 144, 144));

//	audio->playEffect("snd/snd_life_mis.ogg", false, 1.0f, 0, 2.0f);
	auto audio = SimpleAudioEngine::getInstance();
	audio->playEffect("snd/game_miss.ogg", false, 1.0f, 0, 2.0f);
	auto stat= ParticleSystemQuad::create("particle/water.plist");
	stat->setPosition(m_cattle->getPosition()+Vec2(0, 50));
	this->addChild(stat, Z_CATTLE);
	m_cattle->setColor(Color3B(0, 235, 255));
}

void GameScene::removeLife(cocos2d::Ref* pSender) {
	m_cattlehead->removeLastObject(true);
	m_cattle->setColor(Color3B(255, 255, 255));
	m_bogy->setColor(Color3B(255, 255, 255));
	m_err_qst_num.push_back(m_done);
	if (m_cattlehead->count()==0) {
		showEndLayer(false, m_done+1);
	} else {
		initQuestion();
	}

}

void GameScene::addScore() {
	m_score += 10;
	auto label = (Label*)this->getChildByName("label_score");
	char name_temp[100] = {0};
	sprintf(name_temp, "%d", m_score);
	label->setString(name_temp);

	if (m_getfishbath->getChildrenCount() <=0 && m_catch_fish_counter>=3 && m_had_new_get_fish) {
		initQuestion();
	}
	auto audio = SimpleAudioEngine::getInstance();
	audio->playEffect("snd/snd_fish_add.ogg", false, 1.0f, 0, 2.0f);
}

void GameScene::createGetFish(Vec2 pos) {
	auto m_cache=SpriteFrameCache::getInstance();
	auto frame = m_cache->getSpriteFrameByName("getfish00.png");
	auto getFish = GetFish::createWithSpriteFrame(frame);
	getFish->setPosition(pos);
	auto bath_getFish = this->getChildByName("bath_getfish");
	bath_getFish->addChild(getFish);

	auto audio = SimpleAudioEngine::getInstance();
	audio->playEffect("snd/snd_do_right.ogg", false, 1.0f, 0, 1.0f);
}

void GameScene::addCatchFishTimes() {
//	m_catch_fish_counter++;
	m_had_new_get_fish = true;
}

void GameScene::notCatchFish() {
	if (m_catch_fish_counter > 2) {
		initQuestion();
	}
}

Question* GameScene::getQuestion(int which) {
	return (Question*)m_qstlist->getObjectAtIndex(which);
}

void GameScene::initQuestion() {
	if (!m_show_end) {
		m_done++;
		if (m_done > m_total_title_number-1) {
//			showEndLayer(true);
			m_show_end = true;
			showEndLayer(true, m_done);
//			auto endlayer = EndLayer::create(true);
//			endlayer->setQuestion(m_qstlist, m_done);
//			this->getScene()->addChild(endlayer);
//			auto audio = SimpleAudioEngine::getInstance();
//			audio->stopAllEffects();
//
//			int number = m_err_qst_num.size();
////			if (!m_data_json) {
//				int* nums = new int[number];
//				for (int i=0; i<number; i++) {
//					nums[i] = m_err_qst_num.at(i);
////					log("------- showEndLayer num is %d ", nums[i]);
//				}
//				JniToJava::saveWrongQsts((m_done-number)*10, number, nums);
//				delete nums;
////			}
//			m_err_qst_num.clear();
//			JniToJava::savePlanData(m_done-number, m_done, m_time, m_score);
			return;
		}
		initFirstQuestion();
	}
}

void GameScene::showSprite(Ref * pSender) {
	Texture2D* texture_ui_text = (Texture2D*) pSender;
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("fish.plist", texture_ui_text);//通过纹理和.plist文件加入CCSpriteFrameCache
	m_fishbath = SpriteBatchNode::createWithTexture(texture_ui_text);
//	Sprite* raffle_b_friend=CCSprite::createWithSpriteFrameName("raffle_b_friend.png");//尽情使用小图片吧
//		raffle_b_friend->setPosition(ccp(240,200));
//		this->addChild(raffle_b_friend);
//
//		//错误，只能获取ui_text.png的纹理
//		//CCTexture2D* raffle_b_diamond_texture=CCTextureCache::sharedTextureCache()->textureForKey("raffle_b_diamond.png");
//		//正确，可以用这种先获取精灵帧，再从精灵帧中获取ui_text的纹理，以及大小，来构建CCSprite
//		CCSpriteFrame* raffle_b_diamond_spriteframe=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("raffle_b_diamond.png");
//		CCTexture2D* texture=raffle_b_diamond_spriteframe->getTexture();
//		CCRect rect=raffle_b_diamond_spriteframe->getRect();
//		CCSprite* raffle_b_diamond=CCSprite::createWithTexture(texture,rect); //如果纹理需要旋转，setRotation吧
//		raffle_b_diamond->setRotation(false);
//		raffle_b_diamond->setPosition(ccp(240,100));
//		this->addChild(raffle_b_diamond);
}

void GameScene::userChoiceLeffter(cocos2d::Touch* touch, cocos2d::Event* event) {
	auto qst = (Question*)m_qstlist->getObjectAtIndex(m_done);
	if (qst && false == qst->getDone()) {
		for (int i=0; i<3; i++) {
			auto letter = (Leffter*)m_letterlist->getObjectAtIndex(i);
			if (letter) {
				if (letter->onTouchBegan(touch, event)) {
					m_choice_leffter = i;
					break;
				}
			}
		}
	}
}

void GameScene::initFirstQuestion () {
	int which = m_qstOrderlst[m_done];
	Question* qst = Question::create();//jni->getTitleSubject(which);

//	if (m_data_json) {
//		qst = m_read_json->question(which);
//		log(" subject is %s ", qst->getSubject().c_str());
//	} else {
	auto jni = JniToJava::create();
	qst = jni->getTitleSubject(which);
//	}

	if (qst->getSubject().empty()) {
		log("subject is empty");
		auto toast = Toast::create("数据文件出错", 1);
		this->addChild(toast, 64);
	}

	m_qstlist->addObject(qst);
	m_qstlabel->setQuestion(m_done+1, qst);
	m_had_new_get_fish = false;
	m_catch_fish_counter = 0;
	m_cattle->runActionWithAnimation(Cattle::NORMAL);
	m_bogy->runActionWithAnimation(m_bogy->getPosition(), Bogy::NORMAL);
	m_bogy->setRotation(0);
	m_bogy->reset();
	m_counter_label->setVisible(false);
	m_counter_label->setRotation(0);
	hadGraySprite(Color3B(255, 255, 255));
	MenuItemImage* soundItem = getSoundItem();
	if (jni->canSound(which)) {
		soundItem->setVisible(true);
	} else {
		soundItem->setVisible(false);
	}
}

MenuItemImage* GameScene::getSoundItem() {
	return (MenuItemImage*)this->getChildByName("button_menu")->getChildByName("soundItem");
}

void GameScene::loadResource(float dt) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float factor = Director::getInstance()->getContentScaleFactor();

	Sprite* scalespr = Sprite::create("kuang.png");
	Size nineSz = scalespr->getContentSize();

	Scale9Sprite* qstbg = Scale9Sprite::create("kuang.png", Rect(0, 0, nineSz.width, nineSz.height),
			Rect(nineSz.width/4, nineSz.height/3, nineSz.width/2, nineSz.height/3));
	this->addChild(qstbg, Z_GRASS+1, "sprite_qstbg");
	qstbg->setPreferredSize(Size(nineSz.width*1.0f, nineSz.height*1.3f));
	qstbg->setPosition(origin + Vec2(visibleSize.width/2, visibleSize.height*0.13f));

//	MenuItemImage* soundItem = getSoundItem();
//	Size sndSz = soundItem->getContentSize();
//	soundItem->setPosition(origin + Vec2(visibleSize.width/2 + qstbg->getContentSize().width*0.5f - sndSz.width*2.0f, sndSz.height*2.0f));
//	soundItem->setVisible(true);
	SpriteFrameCache* m_cache=SpriteFrameCache::getInstance();

	m_landbath = (SpriteBatchNode*)SpriteBatchNode::create("land_packer.png");
	m_cache->addSpriteFramesWithFile("land_packer.plist");
	this->addChild(m_landbath, Z_LAND_PACKER, "bath_landbath");

	Node* grassprite = this->getChildByName("sprite_grass");

	SpriteFrame* frame = m_cache->getSpriteFrameByName("bogy00.png");
	m_bogy = Bogy::createWithSpriteFrame(frame);
	m_landbath->addChild(m_bogy, 2, "bogy");
	Vec2 m_body_vec = Vec2(origin.x+visibleSize.width/2, origin.y+grassprite->getContentSize().height*0.93);
	m_bogy->setPosition(m_body_vec);

	std::string letternm[] = {"A.png", "B.png", "C.png"};
	frame = m_cache->getSpriteFrameByName("cattle_head.png");
	for (int i=0; i<3; i++) {
		auto letterframe = m_cache->getSpriteFrameByName(letternm[i]);
		Size size = letterframe->getOriginalSize();
		if (visibleSize.width > visibleSize.height) {
			auto letter = Leffter::createWithSpriteFrame(letterframe, Vec2(origin.x+visibleSize.width*(0.3f+0.2f*i), qstbg->getPosition().y+size.height*1.68f));
			m_landbath->addChild(letter, 3);
			m_letterlist->addObject(letter);
		} else {
			auto letter = Leffter::createWithSpriteFrame(letterframe, Vec2(origin.x+visibleSize.width*(0.2f+0.3f*i), origin.y+size.height*3.18f));
			m_landbath->addChild(letter, 3);
			m_letterlist->addObject(letter);
		}

		Sprite* head = Sprite::createWithSpriteFrame(frame);
		m_landbath->addChild(head);
		m_cattlehead->addObject(head);
		Size headSize = head->getContentSize();
		head->setPosition(Vec2(origin.x+headSize.width*(i+1), origin.y+visibleSize.height-headSize.height/2));
	}
	frame = m_cache->getSpriteFrameByName("cattle_fish_top.png");
	Sprite* top = Sprite::createWithSpriteFrame(frame);
	m_landbath->addChild(top);
	top->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height-top->getContentSize().height/2));

	JniToJava* jni = JniToJava::create();
	m_total_title_number = jni->getQuestionNumber();
//	if (m_total_title_number < 0) {
//		m_data_json = true;
//		m_read_json = ReadJSON::create(jni->getJSONPath());
//		m_total_title_number = m_read_json->dataSize();
//	}
	m_qstOrderlst = MyRandom::randomOrder(m_total_title_number);

	Question* qst = nullptr; //= jni->getTitleSubject(m_qstOrderlst[0]);
//	if (m_data_json) {
//		qst = m_read_json->question(m_qstOrderlst[0]);
//	} else {
		qst = jni->getTitleSubject(m_qstOrderlst[0]);
//	}


	int fontSize = 24/factor;
	log("factor is %f, fontsize is %d", factor, fontSize);

	m_qstlist->addObject(qst);
	if (visibleSize.width > visibleSize.height) {
		m_qstlabel = QuestionLabel::createWithSystemFont(qst, "fonts/Readboy-Code.ttf", fontSize, Size(qstbg->getContentSize().width*0.88f, qstbg->getContentSize().height));
	} else {
		m_qstlabel = QuestionLabel::createWithSystemFont(qst, "fonts/Readboy-Code.ttf", fontSize, Size(qstbg->getContentSize().width*0.88f, qstbg->getContentSize().height));
//		m_qstlabel = QuestionLabel::createWithSystemFont(qst, "fonts/readboy_code.ttf", fontSize, Size(qstbg->getContentSize().width*0.88f, qstbg->getContentSize().height));
	}
//	m_qstlabel->setDimensions(qstbg->getContentSize().width*0.88f, qstbg->getContentSize().height);
//	m_qstlabel->setDimensions(visibleSize.width*0.8, visibleSize.height*0.25);
	m_qstlabel->setVerticalAlignment(TextVAlignment::CENTER);
	m_qstlabel->setPosition(qstbg->getPosition());
	this->addChild(m_qstlabel, Z_QST);

	MenuItemImage* soundItem = getSoundItem();
		Size sndSz = soundItem->getContentSize();
		soundItem->setPosition(origin + Vec2(visibleSize.width/2 + qstbg->getContentSize().width*0.5f - sndSz.width*2.0f, m_qstlabel->getPositionY()-m_qstlabel->getContentSize().height*0.3+sndSz.height*0.5));
		if (jni->canSound(0)) {
			soundItem->setVisible(true);
		} else {
			soundItem->setVisible(false);
		}



	Label* label = Label::createWithTTF("0", "fonts/arial.ttf", fontSize*1.3f);
	label->setPosition(Vec2(origin.x + visibleSize.width/2+top->getContentSize().width/5, top->getPosition().y));
	this->addChild(label, Z_LAND_PACKER+1, "label_score");
	SpriteBatchNode* m_cattle_bath = (SpriteBatchNode*)SpriteBatchNode::create("game_cattle.png");
	m_cache->addSpriteFramesWithFile("game_cattle.plist");
	frame = m_cache->getSpriteFrameByName("cattle_init00.png");
	this->addChild(m_cattle_bath, Z_CATTLE, "bath_cattle");
	m_cattle = Cattle::createWithSpriteFrame(frame);
	m_cattle_bath->addChild(m_cattle);
	m_cattle->setPosition(Vec2(origin.x+visibleSize.width*3/4, origin.y+grassprite->getContentSize().height));
	m_cattle->runActionWithAnimation(Cattle::NORMAL);
	m_first_load = true;
//	auto listenerkeyPad = EventListenerKeyboard::create();
//	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
//	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
	this->scheduleOnce(schedule_selector(GameScene::secondLoadRes), 0.04f);
	log("factor is 101 ");
}

void GameScene::secondLoadRes(float dt) {
	log("factor is 111 ");
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	SpriteFrameCache* m_cache=SpriteFrameCache::getInstance();
	m_fishbath = (SpriteBatchNode*)SpriteBatchNode::create("fish.png");
	m_cache->addSpriteFramesWithFile("fish.plist");
	this->addChild(m_fishbath, Z_FISH, "bath_fish");
	log("factor is 11 ");
	createFish(0.1f);
	log("GameScene create m_fishbath ");

	if (m_tip_fish == 0) {
		auto frame = m_cache->getSpriteFrameByName("hand00.png");
		auto m_hand = Hand::createWithSpriteFrame(frame);
		auto node = (Leffter*)m_letterlist->getObjectAtIndex(0);
		m_hand->setPosition(node->getPosition());
		m_hand->runActionWithAnimation(m_bogy->getPosition());
		m_landbath->addChild(m_hand, 4, "sprite_hand");
		m_tip_fish = 1;
	}

	float consz = m_qstlabel->getSystemFontSize()*2;
	m_counter_label = Label::createWithTTF("3", "fonts/arial.ttf", consz, Size(consz*0.65f, consz), TextHAlignment::LEFT, TextVAlignment::BOTTOM);
	m_counter_label->setPosition(m_bogy->getPosition());
	this->addChild(m_counter_label, Z_LAND_PACKER+1, "label_counter");
	m_counter_label->setVisible(false);

	auto net_bath = (SpriteBatchNode*)SpriteBatchNode::create("net.png");
	m_cache->addSpriteFramesWithFile("net.plist");
	this->addChild(net_bath, 3, "bath_net");

	m_second_load = true;

	m_getfishbath = (SpriteBatchNode*)SpriteBatchNode::create("getfish.png");
	m_cache->addSpriteFramesWithFile("getfish.plist");
	this->addChild(m_getfishbath, Z_LAND_PACKER+1, "bath_getfish");
	auto efect = UVSprite::create("humian.png");
	efect->loadShaderVertex("Shaders/uv_vert.vsh", "Shaders/uv_frag.fsh");
	efect->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height-efect->getContentSize().height/2));
	this->addChild(efect, Z_WATER_EFFECT);
	efect->scheduleUpdate();

//	ArmatureDataManager::getInstance()->addArmatureFileInfo("ThirdFish/ThirdFish0.png","ThirdFish/ThirdFish0.plist","ThirdFish/ThirdFish.ExportJson");
//	Armature *armature = Armature::create("ThirdFish");
//	armature->setPosition(Point(visibleSize.width * 0.3, visibleSize.height * 0.7));
//	//播放动画
//	armature->getAnimation()->play("swimming");
//	armature->setScale(0.66f);
//	this->addChild(armature);

}

void GameScene::reset() {

	log("GameScene reset() !!! ");

	m_catch_fish_counter = 0;
	m_qstlist->removeAllObjects();
	m_score = 0;
	m_done = 0;

	auto jni = JniToJava::create();
	jni->reset();

	initFirstQuestion();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto m_cache = SpriteFrameCache::getInstance();
	auto frame = m_cache->getSpriteFrameByName("cattle_head.png");
	for (; m_cattlehead->count()<3; ) {
		int i = m_cattlehead->count();
		auto head = Sprite::createWithSpriteFrame(frame);
		m_landbath->addChild(head);
		m_cattlehead->addObject(head);
		Size headSize = head->getContentSize();
		log("reset cattle head count is %d ", i);
		head->setPosition(Vec2(origin.x+headSize.width*(i+1), origin.y+visibleSize.height-headSize.height/2));
	}

	auto label = (Label*)this->getChildByName("label_score");
	label->setString("0");
}


void GameScene::showEndLayer(bool isSuc, int done) {
	m_show_end = true;
	auto endlayer = EndLayer::create(isSuc);
	endlayer->setQuestion(m_qstlist, done);
	this->getScene()->addChild(endlayer);
	auto audio = SimpleAudioEngine::getInstance();
	audio->stopAllEffects();
	int number = m_err_qst_num.size();
	JniToJava* jniptr = JniToJava::create();
//	if (!m_data_json) {
		int* nums = new int[number];
		for (int i=0; i<number; i++) {
			nums[i] = m_err_qst_num.at(i);
//			log("------- showEndLayer num is %d ", nums[i]);
		}
		jniptr->saveWrongQsts((done-number)*10, number, nums);
		delete nums;
//	}
	m_err_qst_num.clear();
	jniptr->savePlanData(done-number, done, m_time, m_score);
	jniptr->voiceStop();
}


void GameScene::hadGraySprite(const Color3B& color) {
	for (int i=0; i<3; i++) {
		auto letter = (Leffter*)m_letterlist->getObjectAtIndex(i);
		letter->setColor(color);
	}
	auto qstbg = this->getChildByName("sprite_qstbg");
	qstbg->setColor(color);
	m_qstlabel->setColor(color);
//	MenuItemImage* soundItem = getSoundItem();
//	soundItem->setColor(color);
}


void GameScene::optionMovetoBogyCallback(Node* pSender) {
	if (m_choice_leffter >= 0) {
		auto letter = (Leffter*)m_letterlist->getObjectAtIndex(m_choice_leffter);
		if (letter) {
			m_bogy->runActionWithAnimation(Vec2(0, 0), Bogy::BOMB);
			auto qst = (Question*)m_qstlist->getObjectAtIndex(m_done);
			qst->setDone(true);
			qst->setChoice(m_choice_leffter);
			if (qst->getRightId() == m_choice_leffter) {
				doRight();
			} else {
				doWrong();
			}
			letter->reset();
			hadGraySprite(Color3B::GRAY);
		}
	}
	m_choice_leffter = -1;
}

void GameScene::menuSoundCallback(cocos2d::Ref* pSender) {
	JniToJava* jniptr = JniToJava::create();
	jniptr->voiceStart(m_done);
}

void GameScene::menuAgainCallback(cocos2d::Ref* pSender) {
	m_show_end = false;
//	this->getParent()->removeChild(pSender, true);
	reset();
}

void GameScene::menuCloseCallback(cocos2d::Ref* pSender) {
	if (m_show_end) {
		return;
	}
	log("GameScene menuCloseCallback() !!! ");

	JniToJava* jniptr = JniToJava::create();
	jniptr->release();
	jniptr->releaseRe();

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


//} /* namespace cattlefish */
