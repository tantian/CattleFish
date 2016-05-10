#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

/**
@class HelloWorld
@brief Base class for cocos2d::Layer.
@details the main interface of the CattleFish
*/
class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    HelloWorld();
	virtual ~HelloWorld();

    virtual void onEnter();
    virtual void onExit();

    virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    void loadResource(float dt);

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void menuGotoGameCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:

    EventListenerKeyboard* m_key_lstnr;
};

#endif // __HELLOWORLD_SCENE_H__
