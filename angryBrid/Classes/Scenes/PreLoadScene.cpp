#include "PreLoadScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

Scene* Preload::createScene()
{
    // ����һ���������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
    auto scene = Scene::create();
    
    // ��������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
    auto layer = Preload::create();
    
    // ��Preload����Ϊ�ӽڵ���ӵ�����
    scene->addChild(layer);
    
    // ���س�������
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

	//��ʾ�ı���ͼƬ
	auto loading = Sprite::create("loading.png");
	loading->setPosition(visibleSize.width/2,visibleSize.height/2);
	this->addChild(loading);

	//����������
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

	// Ԥ���ر������ֺ���Ч
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