#include "StartScene.h"
#include "utils\ParcitleManager.h"

Scene* Start::createScene()
{
    // 创建一个场景对象，该对象将会由自动释放池管理内存的释放
    auto scene = Scene::create();
    
    // 创建层对象，该对象将会由自动释放池管理内存的释放
    auto layer = Start::create();
    
    // 将Preload层作为子节点添加到场景
    scene->addChild(layer);
    
    // 返回场景对象
    return scene;
}

bool Start::init()
{
	if(!Layer::init()){
		return false;
	}
	
	UserDefault::getInstance()->setIntegerForKey("SuccessLevel",2);
	Size visibleSize= Director::getInstance()->getVisibleSize();
	//背景
	auto startbg = Sprite::create("startbg.png");
	startbg->setPosition(visibleSize.width/2,visibleSize.height/2);
	this->addChild(startbg);
	//文字LOGO
	auto angrybird = Sprite::create("angrybird.png");
	    angrybird->setPosition(visibleSize.width/2, visibleSize.height*0.8);
    this->addChild(angrybird);

	//开始按钮
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
    // TransitionSplitRows 横向剧场动画
    auto transition = TransitionSplitRows::create(2.0f, Level::createScene());
    // 停止开头音乐
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    // 切换到选关场景
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
	//起始坐标
	bird->setPosition(50.0f + CCRANDOM_0_1()*50,70.0f);
	//终点坐标
	Vec2 endPoint = Vec2(visibleSize.width*0.8+CCRANDOM_0_1()*50,65.0f);
	//跳跃高度
	float height = CCRANDOM_0_1()*100+50.0f;
	// 创建一个Jump动作，动作执行时间是2秒，最终位置是endPoint，最大高度是height,跳跃次数1
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

