#include "HelloWorldScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;
//using namespace cattlefish;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld::HelloWorld()
: m_key_lstnr(nullptr)
{
	log("HelloWorld HelloWorld()!!!");
}

HelloWorld:: ~HelloWorld() {
	log(" ~HelloWorld()!!!");
	auto m_txturecache = Director::getInstance()->getTextureCache();
	m_txturecache->removeTextureForKey("begin01.png");
	m_txturecache->removeTextureForKey("begin02.png");
	m_txturecache->removeTextureForKey("main_bg.png");
	m_txturecache->removeTextureForKey("logo.png");

	auto ss = m_txturecache->getCachedTextureInfo();
	log("~HelloWorld() texturecache %s ", ss.c_str());
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto bg = Sprite::create("main_bg.png");
    bg->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2));
    this->addChild(bg, 0);

    this->scheduleOnce(schedule_selector(HelloWorld::loadResource), 0.1f);
    log("HelloWorld init()!!!");
    return true;
}

void HelloWorld::loadResource(float dt) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create("close00.png", "close01.png", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width*2/3 ,
                                origin.y + visibleSize.height-closeItem->getContentSize().height*2/3));

	auto beginItem = MenuItemImage::create("begin01.png",  "begin02.png", CC_CALLBACK_1(HelloWorld::menuGotoGameCallback, this));
	beginItem->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + beginItem->getContentSize().height));

    auto menu = Menu::create(closeItem, beginItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 2);


	auto sprite = Sprite::create("logo.png");
	sprite->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height - sprite->getContentSize().height*3/2));
	this->addChild(sprite, 1);

	SimpleAudioEngine* audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("snd/snd_bg.ogg", true);
	audio->setBackgroundMusicVolume(0.5f);
//	log("-------- HelloWorld loadResource()!!!");
}


void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	log("-------- HelloWold onkeyPressed %d ", keyCode);
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK || keyCode == EventKeyboard::KeyCode::KEY_HOME || keyCode == EventKeyboard::KeyCode::KEY_KP_HOME) {
		log("-------- HelloWold onKeyReleased KEY_BACK ");
		SimpleAudioEngine* audio = SimpleAudioEngine::getInstance();
		audio->stopBackgroundMusic(true);
		Director::getInstance()->end();
	}
}

void HelloWorld::onEnter() {
	Layer::onEnter();
	log("HelloWorld onEnter()!!!");

	if (m_key_lstnr == nullptr) {
		m_key_lstnr = EventListenerKeyboard::create();
		m_key_lstnr->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
		m_key_lstnr->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_key_lstnr,this);
		log("HelloWorld onEnter() create m_key_lstnr ");
	}

}

void HelloWorld::onExit() {
	this->unschedule(schedule_selector(HelloWorld::loadResource));
	Layer::onExit();
	if (m_key_lstnr != nullptr) {
		Director::getInstance()->getEventDispatcher()->removeEventListener(m_key_lstnr);
		m_key_lstnr = nullptr;
	}
	log("HelloWorld onExit()!!!");
}


void HelloWorld::menuGotoGameCallback(cocos2d::Ref* pSender) {
		auto scene = GameScene::createScene();
		//CCDirector::getInstance()->replaceScene(TransitionSlideInR::create(2, scene));

		Director::getInstance()->replaceScene(scene);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
	auto director = Director::getInstance();
	SimpleAudioEngine* audio = SimpleAudioEngine::getInstance();
	audio->stopBackgroundMusic(true);

    director->end();
//	auto m_txturecache = director->getTextureCache();
//	m_txturecache->removeAllTextures();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
