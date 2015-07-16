#include "GameScene.h"

Scene* Game::createScene()
{
    // ´´½¨Ò»¸ö³¡¾°¶ÔÏó£¬¸Ã¶ÔÏó½«»áÓÉ×Ô¶¯ÊÍ·Å³Ø¹ÜÀíÄÚ´æµÄÊÍ·Å
    auto scene = Scene::create();
    
    // ´´½¨²ã¶ÔÏó£¬¸Ã¶ÔÏó½«»áÓÉ×Ô¶¯ÊÍ·Å³Ø¹ÜÀíÄÚ´æµÄÊÍ·Å
    auto layer = Game::create();
    
    // ½«Game²ã×÷Îª×Ó½ÚµãÌí¼Óµ½³¡¾°
    scene->addChild(layer);
    
    // ·µ»Ø³¡¾°¶ÔÏó
    return scene;
}

bool Game::init(){
	Size visibleSize = Director::getInstance()->getVisibleSize();
    screenWidth = visibleSize.width;
    screenHeight = visibleSize.height;

    auto dirt = Sprite::create("bg_dirt.png");
    dirt->setScale(2.0);
    dirt->setPosition(screenWidth/2, screenHeight/2);
    this->addChild(dirt, -2);
    
    // ´´½¨ÉÏ°ë²¿·Ö²ÝµØ
    auto upper = Sprite::create("grass_upper.png");
    upper->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    upper->setPosition(screenWidth/2, screenHeight/2);
    this->addChild(upper, -1);
    
    // ´´½¨ÏÂ°ë²¿·Ö²ÝµØ
    auto lower = Sprite::create("grass_lower.png");
    lower->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    lower->setPosition(screenWidth/2, screenHeight/2);
    this->addChild(lower, 1);
    
    // ´´½¨×ó±ßµÄµØÊó¾«Áé
    auto leftMole = Sprite::create("mole_1.png");
    leftMole->setPosition(168, visibleSize.height / 2 - 160);
    this->addChild(leftMole, 0);
    molesVector.pushBack(leftMole);
    
    // ´´½¨ÖÐ¼äµÄµØÊó¾«Áé
    auto centerMole = Sprite::create("mole_1.png");
    centerMole->setPosition(screenWidth/ 2, screenHeight / 2-160);
    this->addChild(centerMole, 0);
    molesVector.pushBack(centerMole);
    
    // ´´½¨ÓÒ±ßµÄµØÊó¾«Áé
    auto rightMole = Sprite::create("mole_1.png");
    rightMole->setPosition(screenWidth - 168, screenHeight / 2-160);
    this->addChild(rightMole, 0);
    molesVector.pushBack(rightMole);

	sucessScore = 15 + (level*5 - 5);
	totalMoles = 20 + (level * 10 - 10);
	timeBarInterval = 100 / totalMoles;
	popDelay = 0.5 - (level*0.05 - 0.05);

	auto clock = Sprite::create("clock.png");
	clock->setScale(0.3f);
    clock->setPosition(screenWidth*0.2, screenHeight*0.1);
    this->addChild(clock,2);

	timeBar = LoadingBar::create("bar.png");
	timeBar->setScale(0.7f);
	timeBar->setDirection(LoadingBar::Direction::LEFT);
	timeBar->setPercent(100);
	timeBar->setPosition(Vec2(screenWidth*0.55, screenHeight*0.1));
	this->addChild(timeBar,2);

	this->schedule(schedule_selector(Game::andomPopMoles), popDelay);
    this->schedule(schedule_selector(Game::gameOver), popDelay);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch,Event *unused_event){
		Point touchLocation = this->convertTouchToNodeSpace(touch);
		for(Sprite* mole : molesVector){
			if(mole->getTag() == 0){
				continue;
			}
			if(mole->getBoundingBox().containsPoint(touchLocation)){
				auto mallet = Sprite::create("mallet1.png");
				auto malletAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("malletAnimation"));
				mallet->setScale(0.8f);
                mallet->setPosition(mole->getPosition().x+100, mole->getPosition().y+60);
                this->addChild(mallet, 0);
				
				mallet->runAction(Sequence::create(malletAnimation,
                                                   CallFunc::create([=]{
                    // ²¥·ÅµØÊó±»´òÖÐÒôÐ§
                    SimpleAudioEngine::getInstance()->playEffect("ow.caf");
                    // µØÊó±»´òÖÐºó²¥·ÅÁ£×ÓÐ§¹û
                    auto aswoon = ParticleSystemQuad::create("aswoon.plist");
                    aswoon->setPosition(mole->getPosition().x, mole->getPosition().y);
                    this->addChild(aswoon);
                    // É¾³ýÄ¾é³
                    this->removeChild(mallet);
                }), NULL));

				mole->setTag(0);
				score+=1;
				mole->stopAllActions();
				
				auto hitAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("hitAnimation"));
                // µØÊóËõ»ØµØ¶´¶¯×÷
                MoveBy* moveDown = MoveBy::create(0.2f, Point(0, -mole->getContentSize().height));
                EaseInOut* easeMoveDown = EaseInOut::create(moveDown, 3.0f);
                // µØÊóË³ÐòÖ´ÐÐ²¥·Å±»´òÖÐ¶¯»­ºÍËõ»ØµØ¶´¶¯×÷
                mole->runAction(Sequence::create(hitAnimate, easeMoveDown, NULL));
			}
		}
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

	std::string levelString = StringUtils::format("¹Ø¿¨: %i",level);
	levelLabel = Label::createWithSystemFont(levelString,"fonts/Marker Felt.ttf",36);
	levelLabel->setPosition(visibleSize.width*0.10, visibleSize.height-30);
	this->addChild(levelLabel, 3);

	scoreLabel = Label::createWithSystemFont("µÃ·Ö: 0", "fonts/Marker Felt.ttf", 36);
    scoreLabel->setPosition(visibleSize.width*0.30, visibleSize.height-30);
    this->addChild(scoreLabel, 3);
    
    // ¹Ø¿¨µØÊó×ÜÊý
    std::string moleLabelString = StringUtils::format("±¾¹ØµØÊó: %i",totalMoles);
    moleLabel = Label::createWithSystemFont(moleLabelString, "fonts/Marker Felt.ttf", 36);
    moleLabel->setPosition(visibleSize.width*0.55, visibleSize.height-30);
    this->addChild(moleLabel, 3);
    
    // ¹Ø¿¨¹ý¹Ø·ÖÊý
    std::string passLabelString = StringUtils::format("¹ý¹ØÄ¿±ê·Ö: %i",sucessScore);
    passLabel = Label::createWithSystemFont(passLabelString, "fonts/Marker Felt.ttf", 36);
    passLabel->setPosition(visibleSize.width*0.85, visibleSize.height-30);
    this->addChild(passLabel, 3);

	SimpleAudioEngine::getInstance()->playBackgroundMusic("whack.caf",true);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.3f);
	return true;
}

void Game::andomPopMoles(float delta){
    
    // _gameOver±ê¼ÇÎªtrue£¬ÓÎÏ·½áÊø
    if (_gameOver) return;
    
    // ÉèÖÃ·ÖÊý
    scoreLabel->setString(StringUtils::format("±¾¹ØµÃ·Ö: %d",score));
    
    // Ñ­»·µØÊó¼¯ºÏ
    for (auto mole : molesVector)
    {
        // »ñÈ¡Ò»¸öËæ»úÊý£¬Èç¹ûËæ»úÊýÈ¡Ä£3µÈÓÚ0µÄ»°£¬Ôò×¼±¸µ¯³öµØÊó
        int temp = CCRANDOM_0_1()*10000;
        if ( temp % 3 == 0)
        {
            // getNumberOfRunningActionsµÈÓÚ0£¬ËµÃ÷¸ÃµØÊó²¢Ã»ÓÐÖ´ÐÐ¶¯×÷£¬Ò²¾ÍÊÇ»¹Ã»ÓÐ×ê³öÀ´£¬Èç¹û²»µÈÓÚ0£¬ËµÃ÷µØÊóÒÑ¾­×ê³öÀ´ÁË£¬Ôò²»ÔÙÈÃµØÊó×ê³öÀ´¡£
            if (mole->getNumberOfRunningActions() == 0)
            {
                // ×ê³öµØÊó
                popMole(mole);
                // ¼ÆÊ±Æ÷½ø¶ÈÌõ¼õÉÙ
                float percentage = timeBarInterval*totalMoles;
                // Èç¹ûÊ±¼ä²»³¤ÁË£¬½«½ø¶ÈÌõÉèÖÃ³ÉºìÉ«
                if (totalMoles == 9) {
                    timeBar->setColor(Color3B(255, 0, 0));
                    // ²¥·ÅÃëÕë×ß¶¯µÄÉùÒô£¬Ôö¼ÓÓÎÏ·Ê±¼ä½ôÆÈ¸Ð
                    SimpleAudioEngine::getInstance()->playEffect("second.mp3",true);
                }
                timeBar->setPercent(percentage);
            }
        }
    }
}

void Game::popMole(Sprite* mole){
    
    // Ã¿×ê³öÒ»¸öµØÊó£¬µØÊó×ÜÊý¼õ1£¬µ±×ê³öµÄµØÊó³¬¹ýÉè¶¨µÄµØÊó×ÜÊýÊ±£¬º¯Êý½áÊø
    if (totalMoles <= 0) return;
    totalMoles--;
    
    // Èç¹ûµØÊóÉÏÒ»´Î±»´òÖÐÁË£¬ÔÙ×ê³öÀ´µÄÊ±ºò£¬»¹»áÏÔÊ¾±»´òÖÐ¡£ËùÒÔÔÚËüÃ¿´Î´Ó¶´Àï×ê³öÀ´µÄÊ±ºò£¬ÉèÖÃËüµÄÏÔÊ¾Ö¡Îª³õÊ¹Í¼Æ¬¡£
    mole->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mole_1.png"));
    
    // 1.´´½¨Ò»¸öaction£¬ÈÃµØÊóÑØ×ÅYÖá×ê¶´À´¡£
    auto moveUp = MoveBy::create(0.2f, Point(0, mole->getContentSize().height));
    // 2.ÎªÁËÊ¹µÃÒÆ¶¯¸ü¼ÓÕæÊµ£¬ÉÏÃæµÄactionÓÃÒ»¸öEaseInOut¶¯×÷°ü×°ÆðÀ´ÁË¡£Õâ»áÊ¹µÃ×ê¶´µÄ¶¯×÷ÔÚ¿ªÊ¼ºÍ½áÊøµÄÊ±ºòËÙ¶È±È½ÏÂý£¬¿´ÆðÀ´£¬¾ÍºÃÏñµØÊóÔÚ¼ÓËÙºÍ¼õËÙÒ»Ñù¡£
    auto easeMoveUp = EaseInOut::create(moveUp, 3.0f);
    // 3.´´½¨Ò»¸öaction£¬Ê¹µØÊóÄÜ¹»×ê»ØÈ¥¡£ÕâÀïÍ¨¹ýµ÷ÓÃactionµÄreverse·½·¨£¬µÃµ½Ïà·´µÄaction¡£
    auto easeMoveDown = easeMoveUp->reverse();
    /****************µØÊó×ê³öµØ¶´ÑÓ³Ù0.5Ãë***************************/
    // 4.µØÊó×ê³öÀ´ºóÔÝÍ£0.5Ãë
//    auto delay = DelayTime::create(0.5);
    // 5.µØÊó°´Ë³ÐòÖ´ÐÐÖ®Ç°Éè¼ÆµÄ¼¸¸ö¶¯×÷
    //    mole->runAction(Sequence::create( easeMoveUp,delay,easeMoveDown,NULL));
     /*******************************************/
    // 4.ÔÚµØÊó×ê³öÀ´ÒÔºó£¬²¥·ÅµØÊó´óÐ¦µÄ¶¯»­¡£
    auto laughAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("laughAnimation"));
    // 5.µØÊó°´Ë³ÐòÖ´ÐÐÖ®Ç°Éè¼ÆµÄ¼¸¸ö¶¯×÷
    mole->runAction(Sequence::create(
                                     easeMoveUp,
                                     CallFuncN::create(CC_CALLBACK_1(Game::setHit, this)),
                                     laughAnimate,
                                    easeMoveDown,
                                     CallFuncN::create(CC_CALLBACK_1(Game::unHit, this)),NULL)
                    );

}



void Game::setHit(Ref* pSender)
{
    Sprite* mole = (Sprite*)pSender;
    mole->setTag(1);
    // ²¥·ÅµØÊó¡°Ð¦¡±µÄÒôÐ§
    SimpleAudioEngine::getInstance()->playEffect("laugh.caf");
}

// ÔÚµØÊóÐ¦ÍêÖ®ºó£¬ÉèÖÃ²»¿ÉÒÔÇÃ´òµØÊó
void Game::unHit(Ref* pSender)
{
    Sprite* mole = (Sprite*)pSender;
    mole->setTag(0);
}

void Game::gameOver(float delta){
    // ÅÐ¶Ï_totalMolesÊÇ·ñÐ¡ÓÚ0£¬Ð¡ÓÚ0±íÊ¾±¾¹Ø¿¨ËùÓÐµØÊó¶¼ÒÑ¾­×ê³ö
    if (totalMoles <= 0){
        
        // ÅÐ¶Ïµ±ËùÓÐµØÊó¶¼Ëõ»ØµØ¶´Ê±£¬Í£Ö¹ÒôÐ§ºÍ±³¾°ÒôÀÖ£¬ÏÔÊ¾²Ëµ¥
        bool flag = true;
        for (Sprite* mole : molesVector) {
            if(mole->getNumberOfRunningActions() != 0){
                flag = false;
                break;
            }
        }
        
        if (flag) {
            // Í£Ö¹±³¾°ÒôÀÖºÍÒôÐ§
            _gameOver = true;
            // µ±Ç°·ÖÊýÐ¡ÓÚ¹ý¹Ø·ÖÊý£¬ÓÎÏ·Ê§°Ü
            MenuItemImage* goItem = nullptr;
            if (score < sucessScore) {
                // µ±Ç°·ÖÊýÐ¡ÓÚ¹ý¹Ø·ÖÊý£¬ÓÎÏ·Ê§°Ü£¬²Ëµ¥ÏîÎª¼ÌÐøÌôÕ½
                goItem = MenuItemImage::create("btn-continue.png", "btn-continue.png", [&](Ref *sender){
                    Director::getInstance()->replaceScene(Game::createScene());
                });
            }else{
                // µ±Ç°·ÖÊý´óÓÚ¹ý¹Ø·ÖÊý£¬¹ý¹Ø£¬²Ëµ¥ÏîÎª½øÈëÏÂÒ»¹Ø
                goItem = MenuItemImage::create("btn-next.png", "btn-next.png", [&](Ref *sender){
                    // ÓÎÏ·¹Ø¿¨¼Ó1
                    level = level + 1;
                    Director::getInstance()->replaceScene(Game::createScene());
                });
            }
            goItem->setPosition(Vec2(screenWidth / 2, screenHeight / 2+100));
            
            // ÖØÍ·ÔÙÀ´²Ëµ¥Ïî
            auto againItem = MenuItemImage::create("btn-again.png","btn-again.png",
                                                   [&](Ref *sender){
                                                       Director::getInstance()->replaceScene(Start::createScene());
                                                   });
            againItem->setPosition(Vec2(screenWidth / 2, screenHeight / 2-100));
            
            // Í£Ö¹±³¾°ÒôÀÖºÍÒôÐ§
            SimpleAudioEngine::getInstance()->stopAllEffects();
            SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            // Ìí¼Ó²Ëµ¥Ïî
            auto menu = Menu::create(goItem,againItem, NULL);
            // ÉèÖÃ²Ëµ¥ÔÚÆÁÄ»µÄÎ»ÖÃ
            menu->setPosition(Vec2::ZERO);
            // ½«²Ëµ¥Ìí¼Óµ½µ±Ç°²ãµÄ×Ó½Úµã
            this->addChild(menu, 1);
            return;
        }
        
    }
    
}