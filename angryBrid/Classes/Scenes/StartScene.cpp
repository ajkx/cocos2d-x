#include "StartScene.h"
#include "utils\ParcitleManager.h"

Scene* Start::createScene()
{
    // ����һ���������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
    auto scene = Scene::create();
    
    // ��������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
    auto layer = Start::create();
    
    // ��Preload����Ϊ�ӽڵ���ӵ�����
    scene->addChild(layer);
    
    // ���س�������
    return scene;
}

bool Start::init()
{
	if(!Layer::init()){
		return false;
	}
	
	UserDefault::getInstance()->setIntegerForKey("SuccessLevel",2);
	Size visibleSize= Director::getInstance()->getVisibleSize();
	//����
	auto startbg = Sprite::create("startbg.png");
	startbg->setPosition(visibleSize.width/2,visibleSize.height/2);
	this->addChild(startbg);
	//����LOGO
	auto angrybird = Sprite::create("angrybird.png");
	    angrybird->setPosition(visibleSize.width/2, visibleSize.height*0.8);
    this->addChild(angrybird);

	//��ʼ��ť
	auto start = Sprite::create("start.png");
    auto startMenuItem = MenuItemSprite::create(start, start,
                                                CC_CALLBACK_0(Start::startGame, this));
    startMenuItem->setPosition(visibleSize.width/2, visibleSize.height/2);
    auto menu = Menu::create(startMenuItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	auto particleSnow = ParticleManager::getInstance()->particleWithType(snow);
	this->addChild(particleSnow);

	this->schedule(schedule_selector(Start::birdJump),1);
	return true;
}

void Start::startGame() {
    // TransitionSplitRows ����糡����
    auto transition = TransitionSplitRows::create(2.0f, Level::createScene());
    // ֹͣ��ͷ����
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    // �л���ѡ�س���
    Director::getInstance()->replaceScene(transition);
}

void Start::birdJump(float delta)
{
	this->createJumpBrid();
}

void Start::createJumpBrid()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto bird = Sprite::create("bird1.png");
	bird->setScale(CCRANDOM_0_1()*4/10.0f);
	//��ʼ����
	bird->setPosition(50.0f + CCRANDOM_0_1()*50,70.0f);
	//�յ�����
	Vec2 endPoint = Vec2(visibleSize.width*0.8+CCRANDOM_0_1()*50,65.0f);
	//��Ծ�߶�
	float height = CCRANDOM_0_1()*100+50.0f;
	// ����һ��Jump����������ִ��ʱ����2�룬����λ����endPoint�����߶���height,��Ծ����1
	auto actionJump = JumpTo::create(2.0f,endPoint,height,1);
	auto callFuncN = CallFunc::create(CC_CALLBACK_0(Start::birdExplosition,this,bird));
	auto allActions = Sequence::create(actionJump,callFuncN,NULL);
	bird->runAction(allActions);
	this->addChild(bird);
}

void Start::birdExplosition(Ref* pSender)
{
	Node* node = (Node*)pSender;
	auto particleExplosition = ParticleManager::getInstance()->particleWithType(explosition);
	particleExplosition->setPosition(node->getPosition());
	this->addChild(particleExplosition);
	this->removeChild(node);
}

