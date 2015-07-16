#include "GameMenuScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

// 游戏菜单类
Scene* GameMenu::createScene()
{
    // 创建一个场景对象，该对象将会由自动释放池管理内存的释放
    auto scene = Scene::create();
    
    // 创建层对象，该对象将会由自动释放池管理内存的释放
    auto layer = GameMenu::create();
    
    // 将GameMenu层作为子节点添加到场景
    scene->addChild(layer);
    
    // 返回场景对象
    return scene;
}

bool GameMenu::init()
{
	if ( !Layer::init() )
    {
        return false;
    }

	// 获得设备支持的可见OpenGL视图大小（即窗口大小）。
    Size visibleSize = Director::getInstance()->getVisibleSize();
    // 获得可见OpenGL视图的起源点（默认x=0，y=0）。
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(RESOURCESPLIST);
	SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_FILE1);
    SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_FILE2);
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(MUSIC_FILE);


	// “开始游戏”按钮
    auto start_button = Button::create("btn-start.png");
    start_button->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2+75));
    // “开始游戏”按钮添加触摸监听
    start_button->addTouchEventListener([=](Ref* pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            // 左右滑动剧场效果
            auto transition = TransitionSplitRows::create(2.0, Game::createSceneWithLevel(1));
            // push游戏场景
            Director::getInstance()->pushScene(transition);
        }
    });
    this->addChild(start_button,1);

	// “选择关卡”按钮
    auto select_button = Button::create("btn-select.png");
    select_button->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2-75));
    // “选择关卡”按钮添加触摸监听
    select_button->addTouchEventListener([=](Ref* pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            // 右边滑入剧场效果
            auto transition = TransitionSlideInR::create(2.0, LevelManager::createScene());
            // push游戏场景
            Director::getInstance()->pushScene(transition);
        }
    });
    this->addChild(select_button,1);
	return true;
}
