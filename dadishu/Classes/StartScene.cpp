#include "StartScene.h"

Scene* Start::createScene()
{
    // ´´½¨Ò»¸ö³¡¾°¶ÔÏó£¬¸Ã¶ÔÏó½«»áÓÉ×Ô¶¯ÊÍ·Å³Ø¹ÜÀíÄÚ´æµÄÊÍ·Å
    auto scene = Scene::create();
    
    // ´´½¨²ã¶ÔÏó£¬¸Ã¶ÔÏó½«»áÓÉ×Ô¶¯ÊÍ·Å³Ø¹ÜÀíÄÚ´æµÄÊÍ·Å
    auto layer = Start::create();
    
    // ½«Start²ã×÷Îª×Ó½ÚµãÌí¼Óµ½³¡¾°
    scene->addChild(layer);
    
    // ·µ»Ø³¡¾°¶ÔÏó
    return scene;
}

bool Start::init()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(BACKGROUNDPLIST);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(RESOURCESPLIST);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto dirt = Sprite::create("bg_dirt.png");
	dirt->setAnchorPoint(Vec2::ZERO);
	dirt->setScale(2.0f);
	dirt->setPosition(visibleSize.width/2,visibleSize.height/2);
	this->addChild(dirt,-2);

	auto upper = Sprite::create("grass_upper.png");
	upper->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	upper->setPosition(visibleSize.width/2,visibleSize.height/2);
	this->addChild(upper,-1);

    auto lower = Sprite::create("grass_lower.png");
    lower->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    lower->setPosition(visibleSize.width/2, visibleSize.height/2);
    this->addChild(lower, 1);
	
    auto leftMole = Sprite::create("mole_1.png");
    leftMole->setPosition(168, visibleSize.height / 2 - 60);
    this->addChild(leftMole, 0);
    
    // ´´½¨ÖÐ¼äµÄµØÊó¾«Áé
    auto centerMole = Sprite::create("mole_thump4.png");
    centerMole->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(centerMole, 0);
    
    // ´´½¨ÓÒ±ßµÄµØÊó¾«Áé
    auto rightMole = Sprite::create("mole_laugh1.png");
    rightMole->setPosition(visibleSize.width - 168, visibleSize.height / 2);
    this->addChild(rightMole, 0);
    
    // ´´½¨Ä¾é³£¬½«Ä¾é³ÏÔÊ¾ÔÚÖÐ¼äµÄµØÊóÍ·ÉÏ
    auto mallet = Sprite::create("mallet2.png");
    mallet->setScale(0.8f);
    mallet->setPosition(visibleSize.width / 2+80, visibleSize.height / 2+80);
    this->addChild(mallet, 1);
    
    // ¡°·è¿ñ´òµØÊó¡±logo
    auto label1 = Label::createWithSystemFont("·è¿ñ´òµØÊó", "Arial-BoldMT", 80);
    label1->setPosition(visibleSize.width/2, visibleSize.height*0.88);
    label1->setColor(Color3B(255, 0, 0));
    this->addChild(label1,1);

    auto playItem = MenuItemImage::create("btn-start.png", "btn-start.png", CC_CALLBACK_0(Start::startGame, this));
    playItem->setPosition(visibleSize.width/2, visibleSize.height*0.3);
    
	// ´´½¨Ò»¸ö¹Ø±ÕÍ¼±ê£¨MenuItemImage£©
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Start::menuCloseCallback, this));
	// ÉèÖÃ¹Ø±ÕÍ¼±êµÄÎ»ÖÃ
	closeItem->setPosition(Vec2(visibleSize.width - closeItem->getContentSize().width/2 ,
                                 closeItem->getContentSize().height/2));
    
    // ´´½¨Ò»¸ö²Ëµ¥Ïî£¨Menu£©
    auto menu = Menu::create(playItem,closeItem, NULL);
	// ÉèÖÃ²Ëµ¥ÏîÔÚÆÁÄ»µÄÎ»ÖÃ
    menu->setPosition(Vec2::ZERO);
	// ½«²Ëµ¥ÏîÌí¼Óµ½µ±Ç°²ãµÄ×Ó½Úµã
    this->addChild(menu, 1);
    
    // ²¥·Å±³¾°ÒôÀÖ²¢ÉèÖÃ±³¾°ÒôÀÖÒôÁ¿´óÐ¡
    SimpleAudioEngine::getInstance()->playBackgroundMusic("whack.caf",true);
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.3f);
    
    return true;
}

void Start::startGame(){

    // Ô¤¼ÓÔØ±³¾°ÒôÀÖºÍÒôÐ§¼ÓÔØµ½
    SimpleAudioEngine::getInstance()->preloadEffect(LAUGHEFFECT);
    SimpleAudioEngine::getInstance()->preloadEffect(OWEFFECT);
    SimpleAudioEngine::getInstance()->preloadEffect(SECONDEFFECT);
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(BACKGROUNDMUSIC);

	Animation* laughAnimation = getAnimationByName("mole_laugh",0.5f,3);
	Animation* hitAnimation = getAnimationByName("mole_thump", 0.3f, 4);
    // Ä¾é³¶¯»­
    Animation* malletAnimation = getAnimationByName("mallet", 0.15f, 3);
	AnimationCache::getInstance()->addAnimation(laughAnimation, "laughAnimation");
    AnimationCache::getInstance()->addAnimation(hitAnimation, "hitAnimation");
    AnimationCache::getInstance()->addAnimation(malletAnimation, "malletAnimation");
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    // ÇÐ»»µ½ÓÎÏ·Ö÷»­Ãæ
    Director::getInstance()->replaceScene(Game::createScene());
}

Animation* Start::getAnimationByName(std::string animName,float delay,int animNum){
	Animation* animation = Animation::create();
	for(int i = 1; i<=animNum;i++){
		std::string frameName = animName;
		frameName.append(StringUtils::format("%d",i)).append(".png");
		animation->addSpriteFrameWithFile(frameName.c_str());
	}
	animation->setDelayPerUnit(delay);
	animation->setRestoreOriginalFrame(true);
	return animation;

}

void Start::menuCloseCallback(Ref* pSender){
#if(CC_TARGET_PLATFORM ==CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
		MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;

#endif
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


