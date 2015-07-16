#include "FightScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

Scene* Fight::createScene()
{
	auto scene = Scene::create();
	auto layer = Fight::create();
	scene->addChild(layer);
	return scene;
}

bool Fight::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	screenWidth = visibleSize.width;
	screenHeight = visibleSize.height;

	Sprite* bg = Sprite::create("fight.png");
	bg->setPosition(screenWidth/2,screenHeight/2);
	this->addChild(bg);

	player = SpriteBase::create("player_stand_2.png");
	player->setPosition(250,screenHeight/2);
	player->setFlippedX(true);
	isShoot = true;
	monster = SpriteBase::create("eagle_stand_2.png");
	monster->setPosition(screenWidth - 200,screenHeight/2+30);
	monster->setLifeValue(20);

	auto loadingBar = LoadingBar::create("planeHP.png");
	loadingBar->setScale(0.25f);

	loadingBar->setPercent(60);
	loadingBar->setDirection(LoadingBar::Direction::LEFT);
	Vec2 pos = Vec2(monster->getPosition().x,monster->getPosition().y);
	loadingBar->setPosition(Vec2(pos.x+15,pos.y+80));
	monster->setHP(loadingBar);
	this->addChild(monster->getHP(),1);
	monster->setHPInterval(monster->getHP()->getPercent() / (float)monster->getLifeValue());
	this->addChild(player);
	this->addChild(monster);
	auto fightListener = EventListenerTouchOneByOne::create();
	fightListener->onTouchBegan=[](Touch* touch,Event* event){return true;};
	fightListener->onTouchEnded=[=](Touch* touch,Event* event){
		Vec2 touchLocation = touch->getLocation();
		Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);
		if(monster->getBoundingBox().containsPoint(nodeLocation)){
			if(isShoot){
				this->playerAction();
			}
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(fightListener,this);
	SimpleAudioEngine::getInstance()->playBackgroundMusic(FIGHT_FILE);
	this->scheduleUpdate();
	return true; 
}

Animate* Fight::getAnimateByName(std::string animName,float delay,int animNum){
    Animation* animation = Animation::create();
    // ѭ���Ӿ���֡�����л�ȡ��ͼƬ�������Ӧ�ľ���֡��ɶ���
	for(unsigned int i = 1;i<=animNum;i++){
        // ��ȡ����ͼƬ���ƣ�����plane0.png
        std::string frameName = animName;
        frameName.append(StringUtils::format("%d",i)).append(".png");
		// ������ͼƬ���Ϊ����֡��������֡��
		animation->addSpriteFrameWithFile(frameName.c_str());
    }
    // ���ö������ŵ�����
	animation->setDelayPerUnit(delay);
	// �þ�������ڶ���ִ�����ָ������״̬
	animation->setRestoreOriginalFrame(true);
    // ���ض�����������
    Animate* animate = Animate::create(animation);
    return animate;
}

void Fight::playerAction(){
	isShoot = false;
	auto animation = getAnimateByName("player_battle_",0.2f,5);
	auto callfunc = CallFunc::create([=]{
		this->shootFinish();
		this->monsterAction();
	});
	auto sequence = Sequence::create(animation,callfunc,NULL);
	player->runAction(sequence);
}

void Fight::shootFinish(){
	ParticleSystem *bloodSpurts = getParticleWithName(ParticleType::bloodSpurts);
	bloodSpurts->setPosition(monster->getPosition().x - 15,monster->getPosition().y);
	this->addChild(bloodSpurts);
	monster->setLifeValue(monster->getLifeValue() - 7);
	if(monster->getHP() != nullptr){
		monster->getHP()->setPercent(monster->getHPInterval()* monster->getLifeValue());
	}
	auto remove = Sequence::create(
		DelayTime::create(2.0f),
		CallFunc::create([=]{
			this->removeChild(bloodSpurts);
	}),NULL);
	this->runAction(remove);
}

void Fight::monsterAction(){
	float delta = 0.4f;
	auto animAction = getAnimateByName("eagle_battle_",delta,9);
	monster->runAction(animAction);
	Sprite* bullet = Sprite::create("bullet.png");
	bullet->setRotation(270);
	bullet->setScale(2);
	bullet->setPosition(Point(monster->getPosition().x-50,
		monster->getPosition().y-40));
	bullet->setVisible(false);
	this->addChild(bullet);

	auto moveTo = MoveTo::create(0.5f,Vec2(player->getPosition().x,player->getPosition().y+10));
	auto callfunc = CallFunc::create([=]{
		bullet->setVisible(true);
	});
	auto sequence = Sequence::create(DelayTime::create(delta*10),callfunc,moveTo,NULL);
	bullet->runAction(sequence);
	ParticleSystem *magic = getParticleWithName(ParticleType::magic);
	magic->setPosition(Vec2(player->getPosition().x-50,player->getPosition().y+28));
	magic->setVisible(false);
	this->addChild(magic,1,9);
	auto remove = Sequence::create(
		DelayTime::create(delta*11),
		CallFunc::create([=]{
			bullet->stopAllActions();
			this->removeChild(bullet);
			this->getChildByTag(9)->setVisible(true);
	}),
		DelayTime::create(1.0),
		CallFunc::create([=]{
			this->removeChild(this->getChildByTag(9));
			isShoot = true;
	}),NULL);
	monster->runAction(remove);
}

ParticleSystem* Fight::getParticleWithName(ParticleType type){
	ParticleSystem* particle;
	switch(type){
		case ParticleType::bloodSpurts:
			particle = ParticleSystemQuad::create("bloodSpurts.plist");
			particle->setPositionType(ParticleSystem::PositionType::FREE);
			particle->setAutoRemoveOnFinish(true);
			break;
		case ParticleType::magic:
			particle = ParticleSystemQuad::create("magic.plist");
			particle->setBlendAdditive(false);
			particle->setPositionType(ParticleSystem::PositionType::FREE);
			particle->setAutoRemoveOnFinish(true);
			break;
		default:
			break;
	}
	return particle;
}


void Fight::update(float delta)
{
	if(monster->getLifeValue() <= 0){
		this->removeChild(player);
		this->removeChild(monster);
		Director::getInstance()->popScene();
	}

}
