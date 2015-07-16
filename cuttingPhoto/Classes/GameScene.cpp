#include "GameScene.h"

USING_NS_CC;

Scene* Game::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Game::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Game::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	auto photo=cuttingPhotos::create("HelloWorld.png",3,3);
    auto photoSize=photo->getContentSize();
    photo->setPosition(origin.x+visibleSize.width/2-photoSize.width/2,
                       origin.y+visibleSize.height/2-photoSize.height/2);
    
    addChild(photo);
    return true;
}



