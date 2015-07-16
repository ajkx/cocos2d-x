#include "PreLoadScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

Scene* Preload::createScene()
{
    // 创建一个场景对象，该对象将会由自动释放池管理内存的释放
    auto scene = Scene::create();
    
    // 创建层对象，该对象将会由自动释放池管理内存的释放
    auto layer = Preload::create();
    
    // 将Preload层作为子节点添加到场景
    scene->addChild(layer);
    
    // 返回场景对象
    return scene;
}

bool Preload::init()
{
	if(!Layer::init()){
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Preload::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                 origin.y + closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	//显示的背景图片
	auto loading = Sprite::create("loading.png");
	loading->setPosition(visibleSize.width/2,visibleSize.height/2);
	this->addChild(loading);

	//进度条精灵
	auto bar = Sprite::create("progressbar.png");
	progress = ProgressTimer::create(bar);
	progress->setPercentage(0.0f);
	progress->setScale(0.5f);
	progress->setMidpoint(Vec2(0,0.5));
	progress->setBarChangeRate(Vec2(1,0));
	progress->setType(ProgressTimer::Type::BAR);
	progress->setPosition(visibleSize.width/2-50,10);
	this->addChild(progress);

	sourceCount = 3;
	progressInterval = 100.0 / (float)sourceCount;

	// 预加载背景音乐和音效
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(TITLE_FILE);
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(LEVEL_SUCCESS_FILE);
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(LEVEL_FAILED_FILE);
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(BACKGROUND_FILE);
    
    SimpleAudioEngine::getInstance()->preloadEffect(BIRD_FLYING_FILE);
    SimpleAudioEngine::getInstance()->preloadEffect(ICE_FILE);
    SimpleAudioEngine::getInstance()->preloadEffect(PIG_FILE);

	this->schedule(schedule_selector(Preload::load),1.0f);;
	return true;
}

void Preload::load(float delta)
{
	auto ac = ProgressTo::create(1,100-progressInterval*sourceCount);
	progress->runAction(ac);
	if(sourceCount < 0){
		auto scene = Start::createScene();
		Director::getInstance()->replaceScene(scene);
		progress->stopAllActions();
	}
	sourceCount--;
}

void Preload::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}