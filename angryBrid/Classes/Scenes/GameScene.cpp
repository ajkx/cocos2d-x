#include "GameScene.h"
#include "PreloadScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#define SLINGSHOT_POS Point(170, 250) // ��������ʼλ��
#define TOUCH_UNKNOW 0 // ����С���ǣ�0Ϊδ����
#define TOUCH_SELECT 1 // ����С���ǣ�1Ϊ����

static int currentLevel = 1; // Ŀǰ�ؿ�
Scene* Game::createSceneWithLevel(int selectLevel)
{
    // ��ùؿ���
    //currentLevel = selectLevel;
    
    // �ٴ�������������ĳ�����һ��sceneֻ��һ��PhysicsWorld�����µ�����layer����һ��PhysicsWorldʵ����
    
    // ����һ���������󣬸ö�����Զ������ڴ���ͷ�
//    auto scene = Scene::create();
    auto scene = Scene::createWithPhysics();
    // setDebugDrawMask�����ڵ���ʱʹ�ã��������������в��ɼ�����״�ȿ��ӻ���
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
//    PhysicsWorldĬ�����д������ģ�Ĭ�ϴ�СΪVect(0.0f, -98.0f), ��Ҳ����ͨ����setGravity()����������Physics������������
    // �������������������ֵ
    scene->getPhysicsWorld()->setGravity(Vect(0.0f, -200.0f));
    
    // ��������󣬸ö�����Զ������ڴ���ͷ�
    auto layer = Game::create();
    
    // ��Game����Ϊ�ӽڵ���ӵ�����
    scene->addChild(layer);
    
    // ���س�������
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
	// ��ʼ����Ϸ�������Ϊfalse
	isFinish = false;
	isStart = false;
	isCollision = false;
	//����
	auto bgSprite = Sprite::create("bg.png");
    bgSprite->setPosition(screenWidth/2, screenHeight/2);
    this->addChild(bgSprite,-1);

	// ����һ���ڵ㣨ǽ�ڣ�
    auto wall = Node::create();

	Vec2 vers[4] = {Vec2(-visibleSize.width/2,-160),
		Vec2(-visibleSize.width/2,visibleSize.height/2),
		Vec2(visibleSize.width/2,visibleSize.height/2),
		Vec2(visibleSize.width/2,-160)};
	auto body = PhysicsBody::createEdgePolygon(vers,4,PHYSICSBODY_MATERIAL_DEFAULT);
	body->setGroup(1);
	wall->setPosition(visibleSize.width/2,visibleSize.height/2);
	//�������ø���
	wall->setPhysicsBody(body);
	this->addChild(wall);

	    // ���������
    auto leftshot = Sprite::create("leftshot.png");
    leftshot->setPosition(170, 220);
    this->addChild(leftshot);
    
     // �������ұ�
    auto rightshot = Sprite::create("rightshot.png");
    rightshot->setPosition(170, 220);
    this->addChild(rightshot);

	//��������������
	slingShot = SlingShot::create();
	// ���ÿ����
    slingShot->getTexture()->setAliasTexParameters();
    // ������������ʱ�������
    slingShot->setStartPoint1(Vec2(164, 260));
    slingShot->setStartPoint2(Vec2(184, 256));
    // ������������ʱ��һ���յ�
    slingShot->setEndPoint(SLINGSHOT_POS);
    slingShot->setContentSize(Size(480, 320));
    slingShot->setPosition(Vec2(240, 158));
    this->addChild(slingShot,10);
	
	// ����drawNode���������һЩ��ͼ����
	drawNode = DrawNode::create();
	this->addChild(drawNode,10);
	
	// ��ȡ�ؿ�������С����ͱ���
	this->createLevel();
	// ��ʼ��һ��С������������
    this->jump();

	// �����¼�������
	auto gameListener = EventListenerTouchOneByOne::create();
	gameListener->onTouchBegan = [=](Touch* touch,Event* event){
		// Ĭ�ϴ���С����ΪTOUCH_UNKNOW����ʾδ����С��
		touchStatus = TOUCH_UNKNOW;
		Vec2 touchLocation = touch->getLocation();
		Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);
		// ���С�񶼵���ȥ�ˣ�����false
		if(currentBird == nullptr){
			return false;
		}
		// �����bird�Ŀɴ�������
		Rect birdRect = this->rectOfSprite(currentBird);
		// ���С����Ե��������Ҵ���������bird�Ŀɴ�������
		if(currentBird->getReady() && birdRect.containsPoint(nodeLocation)){
			// �޸Ĵ���С����ΪTOUCH_SELECT
			touchStatus = TOUCH_SELECT;
			return true;
		}
		return false;
	};

	gameListener->onTouchMoved = [=](Touch* touch,Event* event){
		if(touchStatus == TOUCH_SELECT){
			Vec2 touchLocation = touch->getLocation();
			Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);
			// ���Ƶ�����Զ���������
			nodeLocation.x = MAX(SLINGSHOT_POS.x-140,nodeLocation.x);
			nodeLocation.x = MIN(SLINGSHOT_POS.x+140,nodeLocation.x);
            nodeLocation.y = MAX(SLINGSHOT_POS.y-50,nodeLocation.y);
            nodeLocation.y = MIN(SLINGSHOT_POS.y+140,nodeLocation.y);
			// ��С��͵�����λ�ö�����ΪnodeLocation
			slingShot->setEndPoint(nodeLocation);
			currentBird->setPosition(nodeLocation);
		}
	};

	gameListener->onTouchEnded = [=](Touch* touch,Event* event){
		// 1.��С��ɳ�ȥ��2. �õ�����λ
		if(touchStatus == TOUCH_SELECT){
			Vec2 touchLocation = touch->getLocation();
			Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);
			// ���õ�����endPoint Ϊ��ʼ��
            slingShot->setEndPoint(SLINGSHOT_POS);

			auto body = currentBird->getPhysicsBody();
			 /**
             setDynamic��ʾ�Ƿ�Ϊ��̬����
             һ������Ƕ�̬������ܹ��ж��˶��е���ײЧ����
             ����Ļ�߽硢ǽ��������Ǿ�̬����
             */
			body->setDynamic(true);
			// nodeLocation.x: ����λ�õ�x-SLINGSHOT_POS.x��������x=���ĳ���
			float offX = nodeLocation.x - SLINGSHOT_POS.x;
            // nodeLocation.y: ����λ�õ�x-SLINGSHOT_POS.y��������x=���ĸ߶�
            float offY = nodeLocation.y - SLINGSHOT_POS.y;
			// ���� = atan2f(�����к���)������/�ڱߣ�
			float radian = atan2f(offY,offX);
			float endX = SLINGSHOT_POS.x - screenWidth/2*cosf(radian) - SLINGSHOT_POS.x;
            // ��SLINGSHOT_POS.yΪԲ�ģ�_screenHeightΪ�뾶��Բ�������ӦԲ�ĵĽǶ�λ�ã���ȥ������y
            float endY = SLINGSHOT_POS.y - screenHeight*sinf(radian) - SLINGSHOT_POS.y;
			// ���ø�������ʣ����������ʰ���������ٶ�
			body->setVelocity(Vec2(endX,endY));
			/************����С��*****************/
            // С�񵯳��������ɾ��
            birdVector.eraseObject(currentBird);
            // 3�������һ��С������������
			auto callFunc = CallFunc::create([=]{
				this->jump();
				// �޸�С���Ƿ���ײ���
				isCollision = false;
			});
			auto action = Sequence::create(DelayTime::create(3),callFunc,NULL);
			this->runAction(action);
            // ����С�������Ч
            SimpleAudioEngine::getInstance()->playEffect(BIRD_FLYING_FILE);
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(gameListener,this);

	// ��ײ���������¼���EventListenerPhysicsContact������
    // ����EventListenerPhysicsContactWithGroup������ͬ��group
	auto contactListener = EventListenerPhysicsContactWithGroup::create(0);
	// EventListenerPhysicsContact���ĸ���ײ�ص�����
    contactListener->onContactBegin = CC_CALLBACK_1(Game::onContactBegin, this);
    contactListener->onContactPreSolve = CC_CALLBACK_2(Game::onContactPreSolve, this);
    contactListener->onContactPostSolve = CC_CALLBACK_2(Game::onContactPostSolve, this);
    contactListener->onContactSeperate = CC_CALLBACK_1(Game::onContactSeperate, this);
	// ���������ײ�¼�������
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
	//��������
	SimpleAudioEngine::getInstance()->playBackgroundMusic(BACKGROUND_FILE);

	this->scheduleUpdate();
	return true;
}

void Game::createLevel()
{
	//��ȡ�ؿ��ļ�
	std::string fileName = StringUtils::format("%d.data",currentLevel);
	//����JSON�ļ�
	JsonParser* parser = JsonParser::create();
	Vector<SpriteData*> spriteDataVector = parser->getAllSpriteWithFile(fileName);
	// ѭ�����������е�ÿһ��Ԫ��SpriteData������Sprite���ͳ�ʼ��Pig����Ice
	for (int i = 0; i < spriteDataVector.size(); i++) {
        auto spriteData = spriteDataVector.at(i);
        switch (spriteData->getTag()) {
            case PIG_TAG:
            {
                // ����pig���������ӵ���ǰ��
                Pig* pig = Pig::create("pig1.png");
                pig->initWithData(spriteData->getX(), spriteData->getY());
                this->addChild(pig);
                break;
            }
            case ICE_TAG:
            {
                // ����ice���������ӵ���ǰ��
                Ice* ice = Ice::create("ice1.png");
                ice->initWithData(spriteData->getX(), spriteData->getY());
                this->addChild(ice);
                break;
            }
            default:
                break;
        }
    }
	    // ����3��С����ӵ���Ļ��һ�����鵱��
    Bird* bird = Bird::create("bird1.png");
    bird->initWithData(320, 186);
    Bird* bird2 = Bird::create("bird1.png");
    bird2->initWithData(280, 186);
    Bird* bird3 = Bird::create("bird1.png");
    bird3->initWithData(240, 186);
    // ��3.0�У�Node��addChild�����У��ж�����body�����������Զ���node��body���õ�scene��PhysicsWorld��ȥ��
    this->addChild(bird);
    this->addChild(bird2);
    this->addChild(bird3);
	// ��3��С����ӵ�С�񼯺ϵ���
	birdVector.pushBack(bird);
	birdVector.pushBack(bird2);
	birdVector.pushBack(bird3);
}

void Game::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	// ���֮ǰ��������������һ���ǳ���Ҫ������������ʱ���кܶ������켣
	drawNode->clear();
	/*  
    ��һ��������Point &from ��ʼλ��
    �ڶ���������const Point &to ����λ��
    ������������float radius �뾶
    ���ĸ�������const Color4F &color ��ɫ
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
	// ���С�񼯺�����Ԫ�ز�����Ϸ�������Ϊfalse
	if(birdVector.size() > 0 && !isFinish){
		// ȡ�����һ��С��Ϊ_currentBird
		currentBird = birdVector.at(0);

		auto jumpTo = JumpTo::create(2,Vec2(170,265),50,1);
		auto callFunc = CallFunc::create([=]{
			// ��Ϸ��ʼ���
			if(!isFinish){
				isStart = true;
			}
			// С���Ƿ�׼���ñ��
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
	// ���صĿɴ�������Ϊ����ʵ�ʴ�С��4��
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
		//���С����ײ�����ŵ�����ë����
		if(spriteA->getTag() ==BIRD_TAG || spriteB->getTag() == BIRD_TAG){
			x = spriteA->getPosition().x;
			y = spriteA->getPosition().y;
			// ���Ŷ���
            this->playHitAnimation(x, y);
		}
		// ���������ײС��������Ч
		if(spriteA->getTag() == ICE_TAG || spriteB->getTag() == ICE_TAG){
			SimpleAudioEngine::getInstance()->playEffect(ICE_FILE);
			if(spriteA->getTag() == PIG_TAG || spriteB->getTag() == PIG_TAG){
				// ������ײС������������ֵ����Ϊ0��֮���������
				if(spriteA->getTag() == ICE_TAG){
					spriteA->setHP(0);
				}
				if(spriteB->getTag() == ICE_TAG){
					spriteB->setHP(0);
				}
			}
		}
		// �����ײС��������Ч
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
		// ��ë
		auto plume = Sprite::create("plume1.png");
		// ��������ë��С
		plume->setScale((float)((CCRANDOM_0_1()*4+1)/10.0f));
		// ������ë�������λ��
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

// �����Ļ�����Ƿ���С�񣬷���true��ʾ�У�����false����ʾû��
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


// �����Ļ�����Ƿ���С������true��ʾ�У�����false����ʾû��
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
	if(message == "����"){
        // ���Ž����ؿ�����
        SimpleAudioEngine::getInstance()->playBackgroundMusic(LEVEL_SUCCESS_FILE);
    }else{
        // ���Ž����ؿ�����
        SimpleAudioEngine::getInstance()->playBackgroundMusic(LEVEL_FAILED_FILE);
    }
	// �����ǰ�ؿ����ѿ��ŵ����ؿ������غ�ؿ���+1
	if(currentLevel == LevelUtils::readLevelFromFile()){
		LevelUtils::writeLevelToFile(currentLevel+1);
	}
	// ֹͣ���ж���
	this->unscheduleUpdate();
	// finishͼƬ����
	auto finishSprite = Sprite::create("finish.png");
	finishSprite->setPosition(screenWidth/2, screenHeight/2);
    finishSprite->setScale(0.7f);
    this->addChild(finishSprite);

	// �ؿ���Label
    std::string levelString = StringUtils::format("��%d��",currentLevel);
    auto levelLabel = Label::createWithSystemFont(levelString, "Marker Felt", 64);
    levelLabel->setColor(Color3B(255, 0, 0));
    levelLabel->setScale(0.6f);
    levelLabel->setPosition(screenWidth/2, screenHeight/2+136);
    this->addChild(levelLabel);

    // ������ʾ��ϢLabel
    auto messageLabel = Label::createWithSystemFont(message, "Marker Felt", 128);
    messageLabel->setColor(Color3B(255, 0, 0));
    messageLabel->setScale(0.6f);
    messageLabel->setPosition(screenWidth/2, screenHeight/2-50);
    this->addChild(messageLabel);

	// ����ʹ��menu.pngͼƬ�Ĳ˵���ò˵�������ѡ��
	auto selectItem = MenuItemImage::create("menu.png","menu.png",
		[=](Ref *sender){
			auto transition = TransitionSplitRows::create(2, Level::createScene());
			Director::getInstance()->replaceScene(transition);
	});
    selectItem->setScale(0.7f);
    selectItem->setPosition(screenWidth/2-140, screenHeight/2-180);

	 // ����ʹ��restart.pngͼƬ�Ĳ˵���ò˵����������¿�ʼ
    auto restartItem = MenuItemImage::create("restart.png", "restart.png",
         [=](Ref *sender){
         // ֹͣ�����ؿ�����
         SimpleAudioEngine::getInstance()->stopBackgroundMusic();
         auto transition = TransitionSplitRows::create(2, Game::createSceneWithLevel(currentLevel));
         Director::getInstance()->replaceScene(transition);
         });
    restartItem->setScale(0.7f);
    restartItem->setPosition(screenWidth/2, screenHeight/2-180);

	// ����ʹ��next.pngͼƬ�Ĳ˵���ò˵������ڽ�����һ��
    auto nextItem = MenuItemImage::create("next.png", "next.png",
         [=](Ref *sender){
         // ֹͣ�����ؿ�����
         SimpleAudioEngine::getInstance()->stopBackgroundMusic();
         auto transition = TransitionSplitRows::create(2, Game::createSceneWithLevel(currentLevel+1));
         Director::getInstance()->replaceScene(transition);
         });
    nextItem->setScale(0.7f);
    nextItem->setPosition(screenWidth/2+140, screenHeight/2-180);
    // ��Ӳ˵�
    auto menu = Menu::create(selectItem,restartItem,nextItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
}

void Game::update(float delay){
	//���С����ײ���Ϊfalse,��С�����е���Ļ��ɾ��
	if(!isCollision){
		float x = currentBird->getPosition().x;
		float y = currentBird->getPosition().y;
		if( x >= screenWidth - 20 || y >= screenHeight - 20){
			this->removeChild(currentBird);
			isCollision = true;
		}
	}
	// ���û��С����Ϸʤ��
	if(!hasPig()){
		gameOver("����");
	}else{
		if(!hasBird()){
			gameOver("����һ��");
		}
	}
}
