#include "GameScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
Scene* Game::createScene()
{
	auto scene = Scene::create();
	auto layer = Game::create();
	scene->addChild(layer);
	return scene;
}

bool Game::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	screenWidth = visibleSize.width;
	screenHeight = visibleSize.height;
	tileMap = TMXTiledMap::create("map.tmx");
	tileMap->setAnchorPoint(Vec2::ZERO);
	tileMap->setPosition(Vec2::ZERO);
	this->addChild(tileMap);

	collidable = tileMap->getLayer("collidable");
	collidable->setVisible(false);

	for(int i = 0; i < tileMap->getMapSize().width;i++){
		Vector<Grid*> inner;
		for(int j = 0; j<tileMap->getMapSize().height;j++){
			Grid *o = Grid::create(i,j);
			inner.pushBack(o);
		}
		_gridVector.push_back(inner);
	}

	for(int i=0; i <_gridVector.size();i++){
		Vector<Grid*> inner = _gridVector.at(i);
		for(int j = 0; j<inner.size();j++){
			Grid *grid = inner.at(j);
			Vec2 tileCoord = Vec2(grid->getX(),grid->getY());
			int tileGid = collidable->getTileGIDAt(tileCoord);
			if(tileGid){
				Value properties = tileMap->getPropertiesForGID(tileGid);
				ValueMap map = properties.asValueMap();
				std::string value = map.at("collidable").asString();
				if(value.compare("true") == 0){
					grid->setPass(false);
				}
			}
		}
	}


	player = Sprite::create("player_stand_1.png");
	player->setFlippedX(true);
	player->setPosition(50,screenHeight/2+240);
	
	tileMap->addChild(player,2);

	auto playerListener = EventListenerTouchOneByOne::create();
	playerListener->onTouchBegan=[](Touch* touch,Event* event){ return true;};
	playerListener->onTouchEnded=[this](Touch* touch,Event* event){
		Vec2 touchLocation = touch->getLocation();
		Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);  //转为tile地图坐标 范围为地图大小
		Vec2 tileCoord = tileCoordForPosition(nodeLocation);

		Point from = tileCoordForPosition(player->getPosition());
		Point to = tileCoordForPosition(nodeLocation);

		int tileGid = collidable->getTileGIDAt(tileCoord);
		if(tileGid){
			Value properties = tileMap->getPropertiesForGID(tileGid);
			ValueMap map = properties.asValueMap();
			std::string value = map.at("collidable").asString();
			if(value.compare("true") == 0){
				return;
			}
		}
		PathArithmetic* pa = PathArithmetic::create();
		Vector<PointDelegate*> pathVector = pa->getPath(from,to,_gridVector);

		if(player->getPosition().x > nodeLocation.x){
			if(player->isFlippedX() == true)player->setFlippedX(false);
		}else{
			if(player->isFlippedX() == false)player->setFlippedX(true);
		}
		if(player->getNumberOfRunningActions() == 0){
			this->playerMoveWithWayPoints(nodeLocation,pathVector);
			setViewPointCenter(nodeLocation);
		}else{
			player->stopAllActions();
			this->stopAllActions();
			this->playerMoveWithWayPoints(nodeLocation,pathVector);
			setViewPointCenter(nodeLocation);
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(playerListener,this);
	this->scheduleUpdate();
	return true;
}

void Game::onEnter(){
    Layer::onEnter();
    // 播放背景音乐
    SimpleAudioEngine::getInstance()->playBackgroundMusic(MUSIC_FILE);
}


Animate* Game::getAnimationByName(std::string animName,float delay,int animNum){
	Animation*  animation = Animation::create();
	for(int i = 1;i<=animNum;i++){
		std::string frameName = animName;
		frameName.append(StringUtils::format("%d",i)).append(".png");
		animation->addSpriteFrameWithFile(frameName.c_str());
	}
	animation->setDelayPerUnit(delay);
	animation->setRestoreOriginalFrame(true);
	Animate* animate = Animate::create(animation);
	return animate;
}

Vec2 Game::tileCoordForPosition(Vec2 position){
	int x = (int)(position.x/(tileMap->getTileSize().width/CC_CONTENT_SCALE_FACTOR())); //X为第几个格子  retina返回2.否则为1
	float pointHeight = tileMap->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
	int y = (int) ((tileMap->getMapSize().height*pointHeight - position.y) / pointHeight);
	return Vec2(x,y);
}

float Game::getPlayerMoveTime(Vec2 startPos,Vec2 endPos){
	Vec2 tileStart = tileCoordForPosition(startPos);
	Vec2 tileEnd = tileCoordForPosition(endPos);
	float duration = 0.15f;  //一个瓦片的移动时间
	duration = duration*sqrtf((tileStart.x - tileEnd.x) * (tileStart.x - tileEnd.x)+ 
								(tileStart.y - tileEnd.y) * (tileStart.y - tileEnd.y));
	return duration;
}

void Game::playerMover(Vec2 position){
	auto animate = getAnimationByName("player_move_",0.2f,4);
	auto repeatanimate = RepeatForever::create(animate);
	player->runAction(repeatanimate);

	auto duration = getPlayerMoveTime(player->getPosition(),position);

	auto moveTo = MoveTo::create(duration,position);
	auto sequence = Sequence::create(moveTo,CallFunc::create([=]{
		player->stopAction(repeatanimate);
		player->setTexture("player_stand_1.png");
	}),nullptr);
	player->runAction(sequence);
	this->setViewPointCenter(position);
}

void Game::setViewPointCenter(Vec2 position){
	int x = MAX(position.x,screenWidth/2);
	int y = MAX(position.y,screenHeight/2);
	x = MIN(x,(tileMap->getMapSize().width*tileMap->getTileSize().width - screenWidth/2));
	y = MIN(y,(tileMap->getMapSize().height*tileMap->getTileSize().height - screenHeight/2));
	Point acPos = Point(x,y);
	Point centerOfView = Point(screenWidth/2,screenHeight/2);
	Point viewPoint = centerOfView - acPos;
	float duration = getPlayerMoveTime(player->getPosition(),position);
	this->runAction(MoveTo::create(duration,viewPoint));
}

void Game::playerMoveWithWayPoints(Vec2 position,Vector<PointDelegate*> path){
	auto animate = getAnimationByName("player_move_",0.4f,4);
	auto repeatanimate = RepeatForever::create(animate);
	player->runAction(repeatanimate);
	Vector<FiniteTimeAction*> actionVector;

	for(int i=0;i<path.size();i++){
		PointDelegate* pd = path.at(i);
		Vec2 p = Point(pd->getX(),pd->getY());
		Vec2 glPoint = locationForTilePos(p);
		MoveTo* moveTo = MoveTo::create(0.4f,glPoint);
		actionVector.pushBack(moveTo);
	}
	auto callfunc = CallFunc::create([=]{
		player->stopAction(repeatanimate);
		player->setTexture("player_stand_1.png");
	});

	actionVector.pushBack(callfunc);
	auto sequence = Sequence::create(actionVector);
	player->runAction(sequence);
}

Vec2 Game::locationForTilePos(Vec2 pos){
	int x = (int)(pos.x*(tileMap->getTileSize().width/CC_CONTENT_SCALE_FACTOR()));
	float pointHeight = tileMap->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
	int y = (int)((tileMap->getMapSize().height * pointHeight) - (pos.y * pointHeight));
	return Point(x,y);
}


Vec2 Game::randomPosition(){
	int randX = CCRANDOM_0_1()*(int)(tileMap->getMapSize().width*tileMap->getTileSize().width - 1)+100;
	int randY = CCRANDOM_0_1()*(int)(tileMap->getMapSize().height*tileMap->getTileSize().height - 1)+120;
	Vec2 position = Point(randX,randY);
	Vec2 tileCoord = tileCoordForPosition(position);
	if(tileCoord.x < 0 || tileCoord.x >= tileMap->getMapSize().width
		||tileCoord.y < 0 || tileCoord.y >=tileMap->getMapSize().height){
			return randomPosition();
	}
	int tileGid = collidable->getTileGIDAt(tileCoord);
	if(tileGid){
		Value property = tileMap->getPropertiesForGID(tileGid);
		ValueMap map = property.asValueMap();
		std::string value = map.at("collidable").asString();
		if(value.compare("true") == 0){
			return randomPosition();
		}else{
			return position;
		}
	}
	return position;
}

Vec2 Game::randomMovePosition(SpriteBase* monster){
	Point position;
	int randX = CCRANDOM_0_1()*199;
	int randY = CCRANDOM_0_1()*99;
	if(monster->getMoveCount() % 2 == 0){
		position = Vec2(monster->getPosition().x + randX, monster->getPosition().y + randY);
		if(!monster->isFlippedX()){
			monster->setFlippedX(true);
		}
	}else{
		position = Vec2(monster->getPosition().x - randX, monster->getPosition().y - randY);
		if(!monster->isFlippedX()){
			monster->setFlippedX(false);
		}
	}

	Vec2 tileCoord = tileCoordForPosition(position);
	if(tileCoord.x < 0 || tileCoord.x >= tileMap->getMapSize().width
		|| tileCoord.y < 0 || tileCoord.y >= tileMap->getMapSize().height){
			return randomMovePosition(monster);
	}

	int tileGid = collidable->getTileGIDAt(tileCoord);
    // 如果随机坐标是不可通过的网格位置，则重新获取
    if (tileGid) {
        // 使用GID来查找指定tile的属性，返回一个Value
        Value properties = tileMap->getPropertiesForGID(tileGid);
        // 返回的Value实际是一个ValueMap
        ValueMap map = properties.asValueMap();
        // 查找ValueMap，判断是否有”可碰撞的“物体，如果有，设置网格对象的isPass变量为false
        std::string value = map.at("collidable").asString();
        if (value.compare("true") == 0) {
            return randomMovePosition(monster);
        }else{
            return position;
        }
    }
    // 如果坐标正常直接返回
    return position;
}


void Game::updateMonsterSprite(float delta){
	if(count % 200 == 0 && monsterVector.size() <= 2){
		SpriteBase* monster = SpriteBase::create("eagle_move_1.png");
		monster->setScale(0.7f);
		monster->setLifeValue(9);
		monster->setMoveCount(1);
		monster->setPosition(randomPosition());
		monsterVector.pushBack(monster);
		tileMap->addChild(monster,2);
		monster->setMove(false);
	}
}

void Game::monsterRepeatedlyMove(float delta){
	for(int i = 0; i < monsterVector.size();i++){
		SpriteBase* monster = monsterVector.at(i);
		if(!monster->isMove()){
			monster->setMove(true);
			Vec2 position = randomMovePosition(monster);
			Vec2 from = tileCoordForPosition(monster->getPosition());
			Vec2 to = tileCoordForPosition(position);
			PathArithmetic* pa = PathArithmetic::create();
			Vector<PointDelegate*> pathVector = pa->getPath(from,to,_gridVector);
			monsterMoveWithWayPoints(pathVector,monster);
		}
	}
}

void Game::monsterMoveWithWayPoints(Vector<PointDelegate*> pathVector,SpriteBase* monster){
	Vector<FiniteTimeAction*> actionVector;
	for(int i = 0; i<pathVector.size();i++){
		PointDelegate* pd = pathVector.at(i);
		Vec2 p = Point(pd->getX(),pd->getY());
		Vec2 glPoint = locationForTilePos(p);
		MoveTo* moveTo = MoveTo::create(1.5f,glPoint);
		actionVector.pushBack(moveTo);
	}

	auto animate = getAnimationByName("eagle_move_",0.5f,4);
	auto repeatanimate = RepeatForever::create(animate);
	monster->runAction(repeatanimate);
	auto callfunc = CallFunc::create([=]{
		monster->stopAction(repeatanimate);
		monster->setMove(false);
		monster->setMoveCount(monster->getMoveCount()+1);
	});
	actionVector.pushBack(callfunc);
	auto sequence = Sequence::create(actionVector);
	monster->runAction(sequence);
}

void Game::collisionDetection(float delta){
	for(int i = 0; i< monsterVector.size();i++){
		SpriteBase* monster = monsterVector.at(i);
		if(monster->getBoundingBox().intersectsRect(player->getBoundingBox())){
			auto transition = TransitionSplitRows::create(2.0,Fight::createScene());
			Director::getInstance()->pushScene(transition);
			tileMap->removeChild(monster);
			monsterVector.eraseObject(monster);
		}
	}
}

void Game::update(float delta){
	count++;
	updateMonsterSprite(delta);
	monsterRepeatedlyMove(delta);
	collisionDetection(delta);
}
