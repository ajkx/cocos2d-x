#include "GameScene.h"
#include "PreloadScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#define SLINGSHOT_POS Point(170, 250) // 弹弓的起始位置
#define TOUCH_UNKNOW 0 // 触碰小鸟标记，0为未触碰
#define TOUCH_SELECT 1 // 触碰小鸟标记，1为触碰

static int currentLevel = 1; // 目前关卡
Scene* Game::createSceneWithLevel(int selectLevel)
{
    // 获得关卡数
    //currentLevel = selectLevel;
    
    // ①创建带物理世界的场景，一个scene只有一个PhysicsWorld，其下的所有layer共用一个PhysicsWorld实例。
    
    // 创建一个场景对象，该对象会自动管理内存的释放
//    auto scene = Scene::create();
    auto scene = Scene::createWithPhysics();
    // setDebugDrawMask经常在调试时使用，它把物理世界中不可见的形状等可视化。
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
//    PhysicsWorld默认是有带重力的，默认大小为Vect(0.0f, -98.0f), 你也可以通过的setGravity()方法来设置Physics的重力参数。
    // ②设置物理世界的重力值
    scene->getPhysicsWorld()->setGravity(Vect(0.0f, -200.0f));
    
    // 创建层对象，该对象会自动管理内存的释放
    auto layer = Game::create();
    
    // 将Game层作为子节点添加到场景
    scene->addChild(layer);
    
    // 返回场景对象
    return scene;
}

bool Game::init()
{
	if ( !Layer::init() )
    {
        return false;
    }

	Size visibleSize = Director::getInstance()->getVisibleSize();
    screenWidth = visibleSize.width;
    screenHeight = visibleSize.height;
	// 初始化游戏结束标记为false
	isFinish = false;
	isStart = false;
	isCollision = false;
	//背景
	auto bgSprite = Sprite::create("bg.png");
    bgSprite->setPosition(screenWidth/2, screenHeight/2);
    this->addChild(bgSprite,-1);

	// 创建一个节点（墙壁）
    auto wall = Node::create();

	Vec2 vers[4] = {Vec2(-visibleSize.width/2,-160),
		Vec2(-visibleSize.width/2,visibleSize.height/2),
		Vec2(visibleSize.width/2,visibleSize.height/2),
		Vec2(visibleSize.width/2,-160)};
	auto body = PhysicsBody::createEdgePolygon(vers,4,PHYSICSBODY_MATERIAL_DEFAULT);
	body->setGroup(1);
	wall->setPosition(visibleSize.width/2,visibleSize.height/2);
	//精灵设置刚体
	wall->setPhysicsBody(body);
	this->addChild(wall);

	    // 弹弓的左边
    auto leftshot = Sprite::create("leftshot.png");
    leftshot->setPosition(170, 220);
    this->addChild(leftshot);
    
     // 弹弓的右边
    auto rightshot = Sprite::create("rightshot.png");
    rightshot->setPosition(170, 220);
    this->addChild(rightshot);

	//画两条弹弓的线
	slingShot = SlingShot::create();
	// 设置抗锯齿
    slingShot->getTexture()->setAliasTexParameters();
    // 设置拉动弹弓时两个起点
    slingShot->setStartPoint1(Vec2(164, 260));
    slingShot->setStartPoint2(Vec2(184, 256));
    // 设置拉动弹弓时的一个终点
    slingShot->setEndPoint(SLINGSHOT_POS);
    slingShot->setContentSize(Size(480, 320));
    slingShot->setPosition(Vec2(240, 158));
    this->addChild(slingShot,10);
	
	// 创建drawNode类用来完成一些绘图工作
	drawNode = DrawNode::create();
	this->addChild(drawNode,10);
	
	// 读取关卡，创建小鸟、猪和冰块
	this->createLevel();
	// 初始化一个小鸟跳到弹弓上
    this->jump();

	// 创建事件监听器
	auto gameListener = EventListenerTouchOneByOne::create();
	gameListener->onTouchBegan = [=](Touch* touch,Event* event){
		// 默认触碰小鸟标记为TOUCH_UNKNOW，表示未触碰小鸟
		touchStatus = TOUCH_UNKNOW;
		Vec2 touchLocation = touch->getLocation();
		Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);
		// 如果小鸟都弹出去了，返回false
		if(currentBird == nullptr){
			return false;
		}
		// 计算出bird的可触碰区域
		Rect birdRect = this->rectOfSprite(currentBird);
		// 如果小鸟可以弹出，并且触摸点是在bird的可触碰区域
		if(currentBird->getReady() && birdRect.containsPoint(nodeLocation)){
			// 修改触碰小鸟标记为TOUCH_SELECT
			touchStatus = TOUCH_SELECT;
			return true;
		}
		return false;
	};

	gameListener->onTouchMoved = [=](Touch* touch,Event* event){
		if(touchStatus == TOUCH_SELECT){
			Vec2 touchLocation = touch->getLocation();
			Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);
			// 控制弹弓最远的拉伸距离
			nodeLocation.x = MAX(SLINGSHOT_POS.x-140,nodeLocation.x);
			nodeLocation.x = MIN(SLINGSHOT_POS.x+140,nodeLocation.x);
            nodeLocation.y = MAX(SLINGSHOT_POS.y-50,nodeLocation.y);
            nodeLocation.y = MIN(SLINGSHOT_POS.y+140,nodeLocation.y);
			// 把小鸟和弹弓的位置都设置为nodeLocation
			slingShot->setEndPoint(nodeLocation);
			currentBird->setPosition(nodeLocation);
		}
	};

	gameListener->onTouchEnded = [=](Touch* touch,Event* event){
		// 1.让小鸟飞出去，2. 让弹弓复位
		if(touchStatus == TOUCH_SELECT){
			Vec2 touchLocation = touch->getLocation();
			Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);
			// 设置弹弓的endPoint 为初始点
            slingShot->setEndPoint(SLINGSHOT_POS);

			auto body = currentBird->getPhysicsBody();
			 /**
             setDynamic表示是否为动态对象。
             一般对象都是动态对象才能够判断运动中的碰撞效果，
             像屏幕边界、墙壁这类才是静态对象。
             */
			body->setDynamic(true);
			// nodeLocation.x: 触碰位置的x-SLINGSHOT_POS.x：弹弓的x=拉的长度
			float offX = nodeLocation.x - SLINGSHOT_POS.x;
            // nodeLocation.y: 触碰位置的x-SLINGSHOT_POS.y：弹弓的x=拉的高度
            float offY = nodeLocation.y - SLINGSHOT_POS.y;
			// 弧度 = atan2f(反正切函数)（对面/邻边）
			float radian = atan2f(offY,offX);
			float endX = SLINGSHOT_POS.x - screenWidth/2*cosf(radian) - SLINGSHOT_POS.x;
            // 以SLINGSHOT_POS.y为圆心，_screenHeight为半径画圆，求出对应圆心的角度位置，减去弹弓的y
            float endY = SLINGSHOT_POS.y - screenHeight*sinf(radian) - SLINGSHOT_POS.y;
			// 设置刚体的速率，物理中速率包括方向和速度
			body->setVelocity(Vec2(endX,endY));
			/************弹出小鸟*****************/
            // 小鸟弹出后从数组删除
            birdVector.eraseObject(currentBird);
            // 3秒后让下一个小鸟跳到弹弓上
			auto callFunc = CallFunc::create([=]{
				this->jump();
				// 修改小鸟是否碰撞标记
				isCollision = false;
			});
			auto action = Sequence::create(DelayTime::create(3),callFunc,NULL);
			this->runAction(action);
            // 播放小鸟飞翔音效
            SimpleAudioEngine::getInstance()->playEffect(BIRD_FLYING_FILE);
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(gameListener,this);

	// 碰撞检测的所有事件由EventListenerPhysicsContact来监听
    // 子类EventListenerPhysicsContactWithGroup监听相同的group
	auto contactListener = EventListenerPhysicsContactWithGroup::create(0);
	// EventListenerPhysicsContact的四个碰撞回调函数
    contactListener->onContactBegin = CC_CALLBACK_1(Game::onContactBegin, this);
    contactListener->onContactPreSolve = CC_CALLBACK_2(Game::onContactPreSolve, this);
    contactListener->onContactPostSolve = CC_CALLBACK_2(Game::onContactPostSolve, this);
    contactListener->onContactSeperate = CC_CALLBACK_1(Game::onContactSeperate, this);
	// 添加物理碰撞事件监听器
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
	//背景音乐
	SimpleAudioEngine::getInstance()->playBackgroundMusic(BACKGROUND_FILE);

	this->scheduleUpdate();
	return true;
}

void Game::createLevel()
{
	//读取关卡文件
	std::string fileName = StringUtils::format("%d.data",currentLevel);
	//解析JSON文件
	JsonParser* parser = JsonParser::create();
	Vector<SpriteData*> spriteDataVector = parser->getAllSpriteWithFile(fileName);
	// 循环遍历集合中的每一个元素SpriteData，根据Sprite类型初始化Pig或者Ice
	for (int i = 0; i < spriteDataVector.size(); i++) {
        auto spriteData = spriteDataVector.at(i);
        switch (spriteData->getTag()) {
            case PIG_TAG:
            {
                // 创建pig精灵对象并添加到当前层
                Pig* pig = Pig::create("pig1.png");
                pig->initWithData(spriteData->getX(), spriteData->getY());
                this->addChild(pig);
                break;
            }
            case ICE_TAG:
            {
                // 创建ice精灵对象并添加到当前层
                Ice* ice = Ice::create("ice1.png");
                ice->initWithData(spriteData->getX(), spriteData->getY());
                this->addChild(ice);
                break;
            }
            default:
                break;
        }
    }
	    // 创建3个小鸟，添加到屏幕和一个数组当中
    Bird* bird = Bird::create("bird1.png");
    bird->initWithData(320, 186);
    Bird* bird2 = Bird::create("bird1.png");
    bird2->initWithData(280, 186);
    Bird* bird3 = Bird::create("bird1.png");
    bird3->initWithData(240, 186);
    // 在3.0中，Node的addChild方法中，有对物理body做处理，它会自动把node的body设置到scene的PhysicsWorld上去。
    this->addChild(bird);
    this->addChild(bird2);
    this->addChild(bird3);
	// 将3个小鸟添加到小鸟集合当中
	birdVector.pushBack(bird);
	birdVector.pushBack(bird2);
	birdVector.pushBack(bird3);
}

void Game::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	// 清除之前所画的线条。这一步非常重要，否则拉弹弓时会有很多线条轨迹
	drawNode->clear();
	/*  
    第一个参数：Point &from 开始位置
    第二个参数：const Point &to 结束位置
    第三个参数：float radius 半径
    第四个参数：const Color4F &color 颜色
    */
    drawNode->drawSegment(Vec2(slingShot->getStartPoint1().x, slingShot->getStartPoint1().y),
                      Vec2(slingShot->getEndPoint().x, slingShot->getEndPoint().y),
					  1, Color4F(1,0,0,1));
    drawNode->drawSegment(Vec2(slingShot->getStartPoint2().x, slingShot->getStartPoint2().y),
                      Vec2(slingShot->getEndPoint().x, slingShot->getEndPoint().y),
                      1, Color4F(1, 0, 0, 1));
}


void Game::jump()
{
	// 如果小鸟集合中有元素并且游戏结束标记为false
	if(birdVector.size() > 0 && !isFinish){
		// 取数组第一个小鸟为_currentBird
		currentBird = birdVector.at(0);

		auto jumpTo = JumpTo::create(2,Vec2(170,265),50,1);
		auto callFunc = CallFunc::create([=]{
			// 游戏开始标记
			if(!isFinish){
				isStart = true;
			}
			// 小鸟是否准备好标记
			if(!currentBird->getReady()){
				currentBird->setReady(true);
			}
		});
		auto allAction = Sequence::create(jumpTo,callFunc,NULL);
		currentBird->runAction(allAction);
	}
}

Rect Game::rectOfSprite(Sprite* sprite)
{
	// 返回的可触碰区域为精灵实际大小的4倍
	return Rect(sprite->getPosition().x - sprite->getContentSize().width/2,
				sprite->getPosition().y - sprite->getContentSize().height/2,
                sprite->getContentSize().width + sprite->getContentSize().width/2,
                sprite->getContentSize().height + sprite->getContentSize().height/2);
}

bool Game::onContactBegin(PhysicsContact& contact)
{
	isCollision = true;
	return true;
}

bool Game::onContactPreSolve(PhysicsContact& contact,PhysicsContactPreSolve& solve)
{
	return true;
}

void Game::onContactPostSolve(PhysicsContact& contact,const PhysicsContactPostSolve& solve)
{
	SpriteBase* spriteA = (SpriteBase*)contact.getShapeA()->getBody()->getNode();
	SpriteBase* spriteB = (SpriteBase*)contact.getShapeB()->getBody()->getNode();
	if(spriteA &&spriteB){
		spriteA->setHP(spriteA->getHP() - 1);
		spriteB->setHP(spriteB->getHP() - 1);
		float x = 0,y = 0;
		//如果小鸟碰撞，播放掉落羽毛动画
		if(spriteA->getTag() ==BIRD_TAG || spriteB->getTag() == BIRD_TAG){
			x = spriteA->getPosition().x;
			y = spriteA->getPosition().y;
			// 播放动画
            this->playHitAnimation(x, y);
		}
		// 如果冰块碰撞小猪，播放音效
		if(spriteA->getTag() == ICE_TAG || spriteB->getTag() == ICE_TAG){
			SimpleAudioEngine::getInstance()->playEffect(ICE_FILE);
			if(spriteA->getTag() == PIG_TAG || spriteB->getTag() == PIG_TAG){
				// 冰块碰撞小猪，将冰块生命值设置为0，之后清除冰块
				if(spriteA->getTag() == ICE_TAG){
					spriteA->setHP(0);
				}
				if(spriteB->getTag() == ICE_TAG){
					spriteB->setHP(0);
				}
			}
		}
		// 如果碰撞小猪，播放音效
		if(spriteA->getTag() == PIG_TAG || spriteB->getTag() == PIG_TAG){
			SimpleAudioEngine::getInstance()->playEffect(PIG_FILE);
		}
		if(spriteA->getHP() <= 0){
			this->removeChild(spriteA);
		}
		if(spriteB->getHP() <= 0){
			this->removeChild(spriteB);
		}
	}
}

void Game::onContactSeperate(PhysicsContact& contact)
{
}

void Game::playHitAnimation(float x,float y)
{
	for(int i = 0; i < 6; i++){
		int range = 2;
		// 羽毛
		auto plume = Sprite::create("plume1.png");
		// 随机获得羽毛大小
		plume->setScale((float)((CCRANDOM_0_1()*4+1)/10.0f));
		// 设置羽毛随机出现位置
		Vec2 pos = Vec2(x+CCRANDOM_0_1()*10*range-10,y+CCRANDOM_0_1()*10*range-10);
		plume->setPosition(pos);
		auto moveTo = MoveTo::create(1,pos);
		auto fadeOut = FadeOut::create(1);
		auto rotateBy = RotateBy::create(1, CCRANDOM_0_1()*180);
		auto spawn = Spawn::create(moveTo,fadeOut,rotateBy, NULL);
        plume->runAction(spawn);
        this->addChild(plume);
	}
}

// 检测屏幕精灵是否还有小鸟，返回true表示有，返回false，表示没有
bool Game::hasBird(){
	Vector<Node*> childrenVector = this->getChildren();
    for (int i = 0; i < childrenVector.size(); i++) {
        Node* node = childrenVector.at(i);
        if(node->getTag() == BIRD_TAG){
            return true;
        }
    }
    return false;
}


// 检测屏幕精灵是否还有小猪，返回true表示有，返回false，表示没有
bool Game::hasPig(){
    Vector<Node*> childrenVector = this->getChildren();
    for (int i = 0; i < childrenVector.size(); i++) {
        Node* node = childrenVector.at(i);
        if(node->getTag() == PIG_TAG){
            return true;
        }
    }
    return false;
}

void Game::gameOver(std::string message)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	if(message == "过关"){
        // 播放结束关卡音乐
        SimpleAudioEngine::getInstance()->playBackgroundMusic(LEVEL_SUCCESS_FILE);
    }else{
        // 播放结束关卡音乐
        SimpleAudioEngine::getInstance()->playBackgroundMusic(LEVEL_FAILED_FILE);
    }
	// 如果当前关卡是已开放的最大关卡，过关后关卡数+1
	if(currentLevel == LevelUtils::readLevelFromFile()){
		LevelUtils::writeLevelToFile(currentLevel+1);
	}
	// 停止所有动作
	this->unscheduleUpdate();
	// finish图片精灵
	auto finishSprite = Sprite::create("finish.png");
	finishSprite->setPosition(screenWidth/2, screenHeight/2);
    finishSprite->setScale(0.7f);
    this->addChild(finishSprite);

	// 关卡数Label
    std::string levelString = StringUtils::format("第%d关",currentLevel);
    auto levelLabel = Label::createWithSystemFont(levelString, "Marker Felt", 64);
    levelLabel->setColor(Color3B(255, 0, 0));
    levelLabel->setScale(0.6f);
    levelLabel->setPosition(screenWidth/2, screenHeight/2+136);
    this->addChild(levelLabel);

    // 过关提示信息Label
    auto messageLabel = Label::createWithSystemFont(message, "Marker Felt", 128);
    messageLabel->setColor(Color3B(255, 0, 0));
    messageLabel->setScale(0.6f);
    messageLabel->setPosition(screenWidth/2, screenHeight/2-50);
    this->addChild(messageLabel);

	// 创建使用menu.png图片的菜单项，该菜单项用于选关
	auto selectItem = MenuItemImage::create("menu.png","menu.png",
		[=](Ref *sender){
			auto transition = TransitionSplitRows::create(2, Level::createScene());
			Director::getInstance()->replaceScene(transition);
	});
    selectItem->setScale(0.7f);
    selectItem->setPosition(screenWidth/2-140, screenHeight/2-180);

	 // 创建使用restart.png图片的菜单项，该菜单项用于重新开始
    auto restartItem = MenuItemImage::create("restart.png", "restart.png",
         [=](Ref *sender){
         // 停止结束关卡音乐
         SimpleAudioEngine::getInstance()->stopBackgroundMusic();
         auto transition = TransitionSplitRows::create(2, Game::createSceneWithLevel(currentLevel));
         Director::getInstance()->replaceScene(transition);
         });
    restartItem->setScale(0.7f);
    restartItem->setPosition(screenWidth/2, screenHeight/2-180);

	// 创建使用next.png图片的菜单项，该菜单项用于进入下一关
    auto nextItem = MenuItemImage::create("next.png", "next.png",
         [=](Ref *sender){
         // 停止结束关卡音乐
         SimpleAudioEngine::getInstance()->stopBackgroundMusic();
         auto transition = TransitionSplitRows::create(2, Game::createSceneWithLevel(currentLevel+1));
         Director::getInstance()->replaceScene(transition);
         });
    nextItem->setScale(0.7f);
    nextItem->setPosition(screenWidth/2+140, screenHeight/2-180);
    // 添加菜单
    auto menu = Menu::create(selectItem,restartItem,nextItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
}

void Game::update(float delay){
	//如果小鸟碰撞标记为false,当小鸟运行到屏幕外删除
	if(!isCollision){
		float x = currentBird->getPosition().x;
		float y = currentBird->getPosition().y;
		if( x >= screenWidth - 20 || y >= screenHeight - 20){
			this->removeChild(currentBird);
			isCollision = true;
		}
	}
	// 如果没有小猪，游戏胜利
	if(!hasPig()){
		gameOver("过关");
	}else{
		if(!hasBird()){
			gameOver("再来一次");
		}
	}
}
