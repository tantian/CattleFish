#include "GameElement.h"
#include "GameScene.h"

//using namespace cattlefish;

Cattle::Cattle() {

}

Cattle::~Cattle() {
//	CC_SAFE_RELEASE_NULL(m_normal);
}

Cattle* Cattle::create() {
	Cattle *sprite = new (std::nothrow) Cattle();
	sprite->autorelease();
	return sprite;
}

Cattle* Cattle::createWithSpriteFrame(SpriteFrame *spriteFrame) {
	Cattle *sprite = new (std::nothrow) Cattle();
	 if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
	 {
	     sprite->autorelease();
	     return sprite;
	 }
	 CC_SAFE_DELETE(sprite);
	 return nullptr;
}


void Cattle::runActionWithAnimation(Animation* animation, int actiontag) {
	auto repeat = CCRepeatForever::create(CCAnimate::create(animation));
	repeat->setTag(actiontag);
	this->runAction(repeat);
}

void Cattle::runActionWithAnimation(int actiontag) {
	if (actiontag == NORMAL) {
		auto animinit = ReadResources::animation("cattle_init", 15,  "anim_cattle_init");
		this->getActionManager()->removeActionByTag(HAPPY, this);
		this->getActionManager()->removeActionByTag(ANGRY, this);
		auto repeat = CCRepeatForever::create(CCAnimate::create(animinit));
		repeat->setTag(actiontag);
		this->runAction(repeat);
	} else if (ANGRY == actiontag) {
		auto animangry = ReadResources::animation("cattle_angry", 10,  "anim_cattle_angry");
		this->getActionManager()->removeActionByTag(NORMAL, this);
		auto repeat = CCRepeatForever::create(CCAnimate::create(animangry));
		repeat->setTag(actiontag);
		this->runAction(repeat);
//		auto seq = (ActionInterval*)CCSequence::create(CCAnimate::create(animangry),
//			CallFuncN::create(CC_CALLBACK_1(Cattle::actionEndCallback, this)), NULL);
//		auto repeat = CCRepeat::create(seq, 1);
//		repeat->setTag(actiontag);
//		this->runAction(repeat);

	} else if (HAPPY == actiontag) {
		auto animhapppy= ReadResources::animation("cattle_happy", 15,  "anim_cattle_happy");
		this->getActionManager()->removeActionByTag(NORMAL, this);
		auto repeat = CCRepeatForever::create(CCAnimate::create(animhapppy));
		repeat->setTag(actiontag);
		this->runAction(repeat);
//		auto seq = (ActionInterval*)CCSequence::create(CCAnimate::create(animhapppy),
//			CallFuncN::create(CC_CALLBACK_1(Cattle::actionEndCallback, this)), NULL);
//		auto repeat = CCRepeat::create(seq, 1);
//		repeat->setTag(actiontag);
//		this->runAction(repeat);
	}
}

void Cattle::actionEndCallback(cocos2d::Ref* pSender) {
	this->runActionWithAnimation(NORMAL);
}

//*************************************************** Leffter *******************************************************//
Leffter* Leffter::createWithSpriteFrame(SpriteFrame *spriteFrame, Vec2 original) {
	Leffter *sprite = new (std::nothrow) Leffter();
	if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
	{
		sprite->m_original = original;
		sprite->setPosition(sprite->m_original);
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

Leffter::Leffter() {

}
Leffter::~Leffter() {

}

bool Leffter::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	bool isdown = false;
	Vec2 touchLac = touch->getLocation();
	if (isTouchDowned(touchLac)) {
		this->setPosition(touchLac);
		this->setColor(Color3B::ORANGE);
		isdown = true;
	}
	return isdown;
}

void Leffter::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {
	if (!m_auto_move) {
		Vec2 touchLac = touch->getLocation();
		if (m_touch_down) {
			this->setPosition(touchLac);
			isUserMove(touchLac);
		} else if (isTouchDowned(touchLac)) {
			this->setPosition(touchLac);
			this->setColor(Color3B::ORANGE);
		}
	}
}


bool Leffter::isUserMove(Vec2 touchPos) {
	bool ismove = false;
	auto size = this->getContentSize();
	if (fabsf(touchPos.x-m_original.x) > size.width/3) {
		ismove = true;
		m_move = true;
	}
	if (fabsf(touchPos.y-m_original.y) > size.height/3) {
		ismove = true;
		m_move = true;
	}
	return ismove;
}

bool Leffter::isTouchDowned(Vec2 touchPos) {
	bool isdown = false;
	auto size = this->getContentSize();
	if (fabsf(touchPos.x-m_original.x) < size.width/2) {
		if (fabsf(touchPos.y-m_original.y)< size.height/2) {
			isdown = true;
			m_touch_down = true;
		}
	}
	return isdown;
}

void Leffter::runActionWithAnimation(Vec2 topos, const CallFuncN* callback) {
	if (!m_auto_move) {
		m_auto_move = true;
		auto moveto = MoveTo::create(0.3f, topos);
		auto seq = (ActionInterval*)CCSequence::create(moveto, callback, NULL);
		auto repeat = CCRepeat::create(seq, 1);
		repeat->setTag(AUTO_MOVE);
		this->runAction(repeat);
	}
}

bool Leffter::isMove() {
	return m_move;
}

bool Leffter::isAutoMove() {
	return m_auto_move;
}

void Leffter::reset() {
	this->getActionManager()->removeActionByTag(AUTO_MOVE, this);
	this->setPosition(m_original);
	m_auto_move = false;
	m_move = false;
	this->setColor(Color3B(255, 255, 255));
}

//*****************************************************WaterEffect***************************************************//
WaterEffect* WaterEffect::createWithSpriteFrame(SpriteFrame *spriteFrame) {
	WaterEffect *sprite = new (std::nothrow) WaterEffect();
	if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
	{
		sprite->autorelease();
		sprite->runActionWithAnimation();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

WaterEffect::WaterEffect() {

}
WaterEffect::~WaterEffect() {

}

void WaterEffect::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {

}
void WaterEffect::runActionWithAnimation() {
	auto animation = ReadResources::animation("watereffect", 20, 0.2);
	auto repeat = CCRepeatForever::create(CCAnimate::create(animation));
	this->runAction(repeat);
}


//********************************************************QuestionLabel*******************************************************************//

QuestionLabel* QuestionLabel::createWithSystemFont(const Question* qst, const std::string& font, float fontSize, const Size& dimensions /* = Size::ZERO */,
		TextHAlignment hAlignment /* = TextHAlignment::LEFT */, TextVAlignment vAlignment /* = TextVAlignment::TOP */)
{
	QuestionLabel* ret = new (std::nothrow) QuestionLabel(nullptr,hAlignment,vAlignment);

    if (ret)
    {
        ret->setSystemFontName(font);
        ret->setSystemFontSize(fontSize);
        ret->setDimensions(dimensions.width, dimensions.height);
        std::string text = ret->qstToString(qst);
        std::string title = "1.";
        title.append(text);
        ret->setString(title);
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}


QuestionLabel::QuestionLabel(FontAtlas *atlas /* = nullptr */, TextHAlignment hAlignment /* = TextHAlignment::LEFT */,
        TextVAlignment vAlignment /* = TextVAlignment::TOP */,bool useDistanceField /* = false */,bool useA8Shader /* = false */)
//: _isOpacityModifyRGB(false)
//, _contentDirty(false)
//, _fontAtlas(atlas)
//, _textSprite(nullptr)
//, _compatibleMode(false)
//, _reusedLetter(nullptr)
//, _additionalKerning(0.0f)
//, _commonLineHeight(0.0f)
//, _lineBreakWithoutSpaces(false)
//, _horizontalKernings(nullptr)
//, _maxLineWidth(0.0f)
//, _labelDimensions(Size::ZERO)
//, _labelWidth(0.0f)
//, _labelHeight(0.0f)
//, _hAlignment(hAlignment)
//, _vAlignment(vAlignment)
//, _currNumLines(-1)
//, _fontScale(1.0f)
//, _useDistanceField(useDistanceField)
//, _useA8Shader(useA8Shader)
//, _effectColorF(Color4F::BLACK)
//, _uniformEffectColor(0)
//, _shadowDirty(false)
//, _insideBounds(true)
{

}
QuestionLabel::~QuestionLabel() {

}

void QuestionLabel::setQuestion(int title, const Question*qst) {
    std::string text = this->qstToString(qst);
    __String* titleS = CCString::createWithFormat("%d.",title);
    std::string txt = titleS->_string;
    txt.append(text);
    this->setString(txt);
}

void QuestionLabel::setSpaceLine(bool isSpaceLine) {
	is_line_qst_ansr = isSpaceLine;
}


std::string QuestionLabel::qstToString(const Question* qst) {
    std::string text = qst->getSubject();
    std::string ntext = "\n";
    text.append(ntext);
//    text.append(ntext);
    if (is_line_qst_ansr) {
    	text.append(ntext);
    }
    std::string nulltext = "A.";
    std::string options = qst->getOption(0);
    nulltext.append(options);
    nulltext.append("    B.");
    options = qst->getOption(1);
    nulltext.append(options);
    nulltext.append("    C.");
    options = qst->getOption(2);
    nulltext.append(options);
    text = text.append(nulltext);
    return text;
}


//************************************************************Bogy****************************************************************//
Bogy* Bogy::createWithSpriteFrame(SpriteFrame *spriteFrame) {
	Bogy *sprite = new (std::nothrow) Bogy();
	if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
	{
		sprite->runActionWithAnimation(Vec2(0, 0), NORMAL);
		sprite->setAnchorPoint(Vec2(0.5, 0.25));
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

Bogy::Bogy() {

}

Bogy::~Bogy() {

}

void Bogy::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {

}

bool Bogy::isInView(Vec2 pos) {
	bool isIn = false;
	Vec2 position = this->getPosition();
	Size size = Size(80, 80);
	if (pos.x > position.x-size.width/2 && pos.x< position.x+size.width/2) {
		if (pos.y > position.y-size.height/2 && pos.y< position.y+size.height/2) {
			isIn = true;
		}
	}
	return isIn;
}

void Bogy::runActionWithAnimation(Vec2 pos, int state) {
	if (state == NORMAL) {
		auto animaNormal = ReadResources::animation("bogy", 7, 0.2f, "bogy_normal");
		auto repeat = CCRepeatForever::create(CCAnimate::create(animaNormal));
		this->getActionManager()->removeActionByTag(BOMB, this);
		this->getActionManager()->removeActionByTag(LAUNCH, this);
		repeat->setTag(NORMAL);
		this->runAction(repeat);
	} else if (BOMB == state) {
		auto animaBomb = ReadResources::animation("bogy", 7, 2, 0.2f, "bogy_bomb");
		auto repeat = CCRepeatForever::create(CCAnimate::create(animaBomb));
		this->getActionManager()->removeActionByTag(NORMAL, this);
		repeat->setTag(BOMB);
		this->runAction(repeat);
	} else if (LAUNCH == state) {
		auto animaLaunch = ReadResources::animation("bogy", 9, 5, "bogy_launch");
		auto seq = (ActionInterval*)CCSequence::create(CCAnimate::create(animaLaunch),
			CallFuncN::create(CC_CALLBACK_1(Bogy::actionEndCallback, this, pos)), NULL);
		auto repeat = CCRepeat::create(seq, 1);
		this->getActionManager()->removeActionByTag(BOMB, this);
		this->getActionManager()->removeActionByTag(LAUNCH, this);
		repeat->setTag(LAUNCH);
		createBullet(pos);
		this->runAction(repeat);
	}
}

void Bogy::runActionTip(Vec2 pos) {
	auto animaLaunch = ReadResources::animation("bogy", 9, 5, "bogy_launch");
	auto seq = (ActionInterval*)CCSequence::create(CCAnimate::create(animaLaunch),
		CallFuncN::create(CC_CALLBACK_1(Bogy::actionEndCallback, this, pos)), NULL);
	auto repeat = CCRepeat::create(seq, 1);
	this->getActionManager()->removeActionByTag(BOMB, this);
	this->getActionManager()->removeActionByTag(LAUNCH, this);
	repeat->setTag(LAUNCH);
	createBullet(pos, true);
	this->runAction(repeat);
}

void Bogy::reset() {
	m_create_net = 0;
}

void Bogy::createBullet(Vec2 pos, bool tip) {
	auto layer = this->getScene()->getChildByName("layer_game");
	auto bath = layer->getChildByName("bath_landbath");
	auto hand = bath->getChildByName("sprite_hand");
	auto bullet = Bullet::createWithFileName("bullet.png");
	if (tip) {
		bullet->setTip(true);
	}
	if (!tip || hand) {
		bath->addChild(bullet);
		bullet->setPosition(this->getPosition());
		bullet->runActionWithAnimation(pos);
	}
}
void Bogy::actionEndCallback(cocos2d::Ref* pSender, Vec2 pos) {
//	auto layer = this->getScene()->getChildByName("layer_game");
//	auto bath = layer->getChildByName("bath_net");
//	auto m_cache=SpriteFrameCache::getInstance();
//	auto frame = m_cache->getSpriteFrameByName("net00.png");
//	auto net = Net::createWithSpriteFrame(frame);
//	net->setPosition(pos);
//	bath->addChild(net);
//	log("bogy lauch end, will create net");
//	net->runActionWithAnimation();
}


//***********************************************************Bullet************************************************************//
Bullet* Bullet::createWithFileName(std::string name) {
	Bullet *sprite = new (std::nothrow) Bullet();
	auto m_cache=SpriteFrameCache::getInstance();
	auto frame = m_cache->getSpriteFrameByName("bullet.png");
	if (sprite && frame && sprite->initWithSpriteFrame(frame))
	{
		sprite->autorelease();
		sprite->setAnchorPoint(Vec2(0.5, 0));
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

Bullet::Bullet() {}
Bullet::~Bullet() {

}

void Bullet::runActionWithAnimation(Vec2 pos) {
	auto moveto = MoveTo::create(0.5f, pos);
	this->setRotation(MyRotate::rotate(pos, this->getPosition()));
	auto seq = (ActionInterval*)CCSequence::create(moveto,
		CallFuncN::create(CC_CALLBACK_1(Bullet::actionEndCallback, this, pos)), NULL);
	auto repeat = CCRepeat::create(seq, 1);
	this->runAction(repeat);
}
void Bullet::actionEndCallback(Ref* pSender, Vec2 pos) {
	this->removeFromParentAndCleanup(true);

	auto layer = Director::getInstance()->getRunningScene()->getChildByName("layer_game");
	auto bath = layer->getChildByName("bath_net");
	auto m_cache=SpriteFrameCache::getInstance();
	auto frame = m_cache->getSpriteFrameByName("net00.png");
	auto net = Net::createWithSpriteFrame(frame);
	net->setPosition(pos);
	bath->addChild(net);
//	log("bogy lauch end, will create net");
	if (!m_tip) {
		net->runActionWithAnimation();
	} else {
		auto land_bath = layer->getChildByName("bath_landbath");
		auto hand = land_bath->getChildByName("sprite_hand");
		if (hand) {
			net->runActionTipAnimation();
		} else {
			net->removeFromParentAndCleanup(true);
		}
	}
}

void Bullet::setTip(bool tip) {
	m_tip = tip;
}


//****************************************************************Net********************************************************//

Net* Net::createWithSpriteFrame(SpriteFrame *spriteFrame) {
	Net *sprite = new (std::nothrow) Net();
	if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

Net::Net() {

}

Net::~Net() {

}

void Net::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {

}

bool Net::isInView(Vec2 pos) {
	bool reback = false;
	Vec2 position = this->getPosition();
	Size size = this->getContentSize();
	if (pos.x > position.x-size.width/2 && pos.x< position.x+size.width/2) {
		if (pos.y > position.y-size.height/2 && pos.y< position.y+size.height/2) {
			reback = true;
		}
	}
	return reback;
}
void Net::runActionWithAnimation() {
	auto anim = ReadResources::animation("net", 7, "anim_net");
	auto seq = (ActionInterval*)CCSequence::create(CCAnimate::create(anim),
		CallFuncN::create(CC_CALLBACK_1(Net::actionEndCallback, this)), NULL);
	auto repeat = CCRepeat::create(seq, 1);
	repeat->setTag(1);
	this->runAction(repeat);
}

void Net::runActionTipAnimation() {
	auto anim = ReadResources::animation("net", 7, "anim_net");
	auto seq = (ActionInterval*)CCSequence::create(CCAnimate::create(anim),
		CallFuncN::create(CC_CALLBACK_1(Net::callbackRemoveByself, this)), NULL);
	auto repeat = CCRepeat::create(seq, 1);
	this->runAction(repeat);
}

void Net::actionEndCallback(Node* pSender) {
	this->getActionManager()->removeActionByTag(1, this);
	auto layer = (GameScene*)this->getScene()->getChildByName("layer_game");
	auto bath_fish = layer->getChildByName("bath_fish");
	auto vector = bath_fish->getChildren();
	int childCount = bath_fish->getChildrenCount();
	Vector<Node*> fish_innet;
	int innet=0;

	for (int i=0; i<childCount; i++) {
		auto fish = (Node*)vector.at(i);
		if (isInView(fish->getPosition())) {
			fish_innet.pushBack(fish);
			innet++;
		}
	}

	if (!fish_innet.empty()) {
		for (int i=0; i<innet; i++) {
			auto fish = fish_innet.at(i);
			layer->createGetFish(fish->getPosition());
			bath_fish->removeChild(fish);
			vector.eraseObject(fish);
		}
		if (vector.capacity() != vector.size()) {
			log("fish capacity is %d, fish number is %d ", vector.capacity(), vector.size());
		}
		fish_innet.clear();
	} else {
		layer->notCatchFish();
	}
	layer->addCatchFishTimes();
	this->removeFromParentAndCleanup(true);
}

void Net::callbackRemoveByself(Node* pSender) {
	this->removeFromParentAndCleanup(true);
}

//****************************************************************GetFish********************************************************//
GetFish* GetFish::createWithSpriteFrame(SpriteFrame *spriteFrame) {
	GetFish *sprite = new (std::nothrow) GetFish();
	if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
	{
		sprite->autorelease();
		sprite->runActionWithAnimation();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

GetFish::GetFish() {

}
GetFish::~GetFish() {

}

void GetFish::runActionWithAnimation() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto anim = ReadResources::animation("getfish", 15, "anim_getfish");
	auto repeatForver = CCRepeatForever::create(CCAnimate::create(anim));
	repeatForver->setTag(1);
	this->runAction(repeatForver);

	Vec2 to = Vec2(origin.x+visibleSize.width/2-this->getContentSize().width/2, origin.y+visibleSize.height-this->getContentSize().height/3);
	float rand = rand_0_1();
	int randint = rand*100;
	float gapt = randint/100.0f;
	auto moveto = MoveTo::create(1.0f+gapt, to);

	auto seq = (ActionInterval*)CCSequence::create(moveto,
		CallFuncN::create(CC_CALLBACK_1(GetFish::actionEndCallback, this)), NULL);
	auto repeat = CCRepeat::create(seq, 1);
	repeat->setTag(2);
	this->runAction(repeat);
}

void GetFish::actionEndCallback(Ref* pSender) {
	this->getActionManager()->removeActionByTag(1, this);
	this->getActionManager()->removeActionByTag(2, this);
	auto layer = this->getScene()->getChildByName("layer_game");
	auto layer_game = (GameScene*)(layer);
	this->removeFromParentAndCleanup(true);
	layer_game->addScore();
}

//****************************************************************Hand********************************************************//
Hand* Hand::createWithSpriteFrame(SpriteFrame *spriteFrame) {
	Hand *sprite = new (std::nothrow) Hand();
	if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
	{
		sprite->autorelease();
//		sprite->runActionWithAnimation();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

Hand::Hand() {

}
Hand::~Hand() {

}

void Hand::setPosition(const Vec2 &position) {
	auto size = this->getContentSize();
	m_orignal_vec2 = Vec2(position.x+size.width/5, position.y-size.height/5);
	Sprite::setPosition(m_orignal_vec2);
}

void Hand::runActionWithAnimation(Vec2 srcto) {
//	log("-------- srcto befor ");
	auto anim = ReadResources::animation("hand", 2, "anim_hand");
//	log("-------- srcto aftor ");
	auto size = this->getContentSize();
	Vec2 to = Vec2(srcto.x+size.width/5, srcto.y-size.height*0.2f);
	auto moveto = MoveTo::create(1.0f, to);
	auto stand = MoveTo::create(0.3f, to);
	auto touch = Repeat::create(Animate::create(anim), 3);
	auto seq = (ActionInterval*)Sequence::create(touch, moveto, stand,
			CallFuncN::create(CC_CALLBACK_1(Hand::actionNotTouchCallback, this)), stand,
			CallFuncN::create(CC_CALLBACK_1(Hand::actionEndCallback, this)), NULL);
	auto repeatForver = RepeatForever::create(seq);
	this->runAction(repeatForver);
}

void Hand::runActionTipFish(Vec2 srcto) {
	m_tip_fish = 3;
	auto anim = ReadResources::animation("hand", 2, "anim_hand");
	auto size = Director::getInstance()->getVisibleSize();
	auto touch = CCRepeat::create(CCAnimate::create(anim), 1);

	Vec2 to = Vec2(srcto.x, srcto.y-size.height*0.1f);
	this->setPosition(to);
	auto moveto = MoveTo::create(0.5f, to);
	auto stand = MoveTo::create(0.5f, to);

	Vec2 to1 = Vec2(to.x+size.width/4, to.y+size.height*0.2f);
	auto moveto1 = MoveTo::create(0.5f, to1);
	auto stand1 = MoveTo::create(0.5f, to1);

	Vec2 to2 = Vec2(to.x+size.width/2, to.y+size.height*0.1f);
	auto moveto2 = MoveTo::create(0.5f, to2);
	auto stand2 = MoveTo::create(0.5f, to2);

	auto seq = (ActionInterval*)CCSequence::create(moveto, touch,
			CallFuncN::create(CC_CALLBACK_1(Hand::actionTipNetCallback, this)), stand,
			moveto1, touch,
			CallFuncN::create(CC_CALLBACK_1(Hand::actionTipNetCallback, this)), stand1,
			moveto2, touch,
			CallFuncN::create(CC_CALLBACK_1(Hand::actionTipNetCallback, this)), stand2,
			CallFuncN::create(CC_CALLBACK_1(Hand::actionTipFishEndCallback, this)), NULL);
	auto repeatForver = CCRepeatForever::create(seq);
	this->runAction(repeatForver);
}


void Hand::actionTipNetCallback(Ref* pSender) {
	auto layer = (GameScene*)Director::getInstance()->getRunningScene()->getChildByName("layer_game");
	auto m_cache=SpriteFrameCache::getInstance();
	layer->tipFiredBullet(this->getPosition());
	auto frame = m_cache->getSpriteFrameByName("hand00.png");
	this->setSpriteFrame(frame);
	auto counterLabel = (Label*)layer->getChildByName("label_counter");
	m_tip_fish--;
    __String* counters = CCString::createWithFormat("%d", (m_tip_fish));
    counterLabel->setString(counters->_string);
}

void Hand::actionNotTouchCallback(Ref* pSender) {
	auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("hand00.png");
	this->setSpriteFrame(frame);
}

void Hand::actionEndCallback(Ref* pSender) {
	Sprite::setPosition(m_orignal_vec2);
}

void Hand::actionTipFishEndCallback(Ref* pSender) {
	m_tip_fish = 3;
	Sprite::setPosition(m_orignal_vec2);
	auto layer = (GameScene*)Director::getInstance()->getRunningScene()->getChildByName("layer_game");
	auto counterLabel = (Label*)layer->getChildByName("label_counter");
    __String* counters = CCString::createWithFormat("%d", (m_tip_fish));
    counterLabel->setString(counters->_string);
}

void Hand::removeSelfCallback(Ref* pSender) {
	this->removeFromParentAndCleanup(true);
}

//********************************************************************Toast***********************************************************//
Toast* Toast::create(const std::string& text, float showdt) {
    auto ret = new (std::nothrow) Toast();
    if (ret && ret->init(text, showdt)) {
    	ret->autorelease();
    	return ret;
    }

    return nullptr;
}


Toast::Toast() {

}

Toast::~Toast() {

}

bool Toast::init(const std::string& text, int dt) {
	int fontsize = 18;
	auto msgLabel = Label::createWithSystemFont(text, "fonts/arial.ttf", fontsize);
	if (!msgLabel) {
		return false;
	}

	Size sz = Size(((int)text.length()-1)*fontsize/2, fontsize*2);
	if (!LayerColor::initWithColor(Color4B::WHITE, sz.width, sz.height)) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	this->setPosition(origin + Vec2((visibleSize.width-sz.width)/2, (visibleSize.height-sz.height)/3));
	msgLabel->setColor(Color3B::BLACK);
	this->addChild(msgLabel);

	msgLabel->setPosition(Vec2(sz.width/2, sz.height/2));
	this->scheduleOnce(schedule_selector(Toast::end), dt);
	return true;
}

void Toast::end(float dt) {
	this->removeFromParent();
	Director::getInstance()->end();
}

//*************************************************************MyRotate****************************************************************//
float MyRotate::rotate(Vec2 dstvec, Vec2 orivec) {
	Vec2 movevector = dstvec - orivec;
	Vec2 normalizedVector = ccpNormalize(movevector);
	float radians = atan2(normalizedVector.x, normalizedVector.y);
	float degree = CC_RADIANS_TO_DEGREES(radians);
	return degree;
}

//*************************************************************MyString****************************************************************//
std::string MyString::getLeffter(int which) {
	std::string name_temp[] = {"A", "B", "C", "D"};
	return name_temp[which];
}

std::string MyString::getString(int which) {
    __String* number = CCString::createWithFormat("%d", which);
    std::string txt = number->_string;
    return txt;
}

// ****************************************************MyRandom************************************************************//

int MyRandom::randomNumber(int start,int end)
{
    return CCRANDOM_0_1()*(end-start)+start;
}

//__Array* MyRandom::randomOrder(int total_number) {
//	__Array* randlst = __Array::createWithCapacity(total_number);
//	if (total_number > 0) {
//		for (int i=0; i<total_number; i++) {
//			randlst->addObject(i);
//		}
//		for (int i=0; i<total_number; i++) {
//			float rand0 = rand_0_1();
//			float rand1 = rand_0_1();
//			int randInt0 = rand0*total_number;
//			int randInt1 = rand1*total_number;
//			if (randInt0 != randInt1) {
//				randlst->exchangeObjectAtIndex(randInt0, randInt1);
//			}
//			log("random Order number is %d ", randlst->getObjectAtIndex(i));
//		}
//	}
//
//	return randlst;
//}


int* MyRandom::randomOrder(int totalnumber) {
	int* rand = new int[totalnumber];
	if (totalnumber > 0) {
		for (int i=0; i<totalnumber; i++) {
			rand[i] = i;
		}
		for (int i=0; i<totalnumber; i++) {
			float rand0 = rand_0_1();
			float rand1 = rand_0_1();
			int randInt0 = rand0*totalnumber;
			int randInt1 = rand1*totalnumber;
			if (randInt0 != randInt1) {
				int temp_int = rand[randInt0];
				rand[randInt0] = rand[randInt1];
				rand[randInt1] = temp_int;
			}
		}
//		for (int i=0; i<totalnumber; i++) {
//			log(" %d random Order number is %d ", i, rand[i]);
//		}
	}

	return rand;
}

Vector<__Integer*> MyRandom::randomVector(int totalnumber) {
	Vector<__Integer*> randVector;
	if (totalnumber > 1) {
		Vector<__Integer*> orderVector;
		for (int i=0; i<totalnumber; i++) {
			orderVector.pushBack(__Integer::create(i));
		}
		for (int i=0; i<totalnumber; i++) {
			float randf = rand_0_1();
			int randInt = randf * orderVector.size();
			randVector.pushBack(orderVector.at(randInt));
			orderVector.erase(randInt);
		}
		orderVector.clear();
	}
	return randVector;
}


//****************************************ReadResources******************************************************************//

Animation* ReadResources::animation(const char* name, int start, int length, float unitdelay) {
	SpriteFrameCache* frame_cache = SpriteFrameCache::getInstance();
	auto anim = CCAnimation::create();
	char name_temp[100] = {0};
	char name_postfix[100] = {0};
	for (int index=start; index<start+length; index++) {
		strcpy(name_temp, name);
		if (index<10) {
			sprintf(name_postfix, "0%d.png", index);
		} else {
			sprintf(name_postfix, "%d.png", index);
		}
		strcat(name_temp, name_postfix);
//		log("---- name is %s , %s", name_temp, name_postfix);
		SpriteFrame* frame = frame_cache->getSpriteFrameByName(name_temp);
		anim->addSpriteFrame(frame);
	}
//	delete name_postfix;
//	delete name_temp;
	anim->setDelayPerUnit(unitdelay);
	return anim;
}

Animation*ReadResources:: animation(const char* name, int length, float unitdelay) {
	return animation(name, 0, length, unitdelay);
}

Animation* ReadResources::animation(const char* name, int length, const char* saveTag) {
	return animation(name, 0, length, saveTag);
}

Animation* ReadResources::animation(const char* name, int length, float unitdelay, const char* saveTag) {
	return animation(name, 0, length, unitdelay, saveTag);
}

Animation* ReadResources::animation(const char* name, int start, int length, const char* saveTag) {
	return animation(name, start, length, 0.1f, saveTag);
}

Animation* ReadResources::animation(const char* name, int start, int length, float unitdelay, const char* saveTag) {
	auto anim_cache = CCAnimationCache::getInstance();
	auto anim = anim_cache->getAnimation(saveTag);
	if (anim == nullptr) {
		anim = animation(name, start, length, unitdelay);
		if (anim) {
			anim_cache->addAnimation(anim, saveTag);
		}
	}
	return anim;
}


//Animation* ReadResources::animation(const char* name, int length, const std::string& saveTag) {
//	return animation(name, 0, length, saveTag);
//}
//
//Animation* ReadResources::animation(const char* name, int length, float unitdelay, const std::string& saveTag) {
//	return animation(name, 0, length, unitdelay, saveTag);
//}
//
//Animation* ReadResources::animation(const char* name, int start, int length, const std::string& saveTag) {
//	return animation(name, start, length, 0.1f, saveTag);
//}
//
//Animation* ReadResources::animation(const char* name, int start, int length, float unitdelay, const std::string& saveTag) {
//	auto anim_cache = CCAnimationCache::getInstance();
//	auto anim = anim_cache->getAnimation(saveTag);
//	if (anim == nullptr) {
//		anim = animation(name, start, length, unitdelay);
//	}
//	return anim;
//}


/**********************************************************************************************************************/
//void (*ucnv_convert)(const char *, const char *, char * , int32_t , const char *, int32_t,int32_t*) = 0;
//ucnv_convert = (void (*)(const char *, const char *, char * , int32_t , const char *, int32_t,int32_t*))dlsym(libFile, "ucnv_convert_3_8");
//ucnv_convert("utf-8", "gb2312", str, len * 2 + 10, gb2312, len, &err_code);
//
//int (*JniGetVersion)() = 0;
//	public static native int JniCreate(String resFilename);
//	private static native int JniDestory();
//	public static native int JniStop();
//	public static native int JniSpeak(String text);
//	public static native int JniSetParam(int paramId,int value);
//	public static native int JniGetParam(int paramId);
//	public static native int JniIsPlaying();
//	public static native boolean JniIsCreated();
