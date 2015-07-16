#include "LevelManager.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

Scene* LevelManager::createScene()
{
    // 创建一个场景对象，该对象将会由自动释放池管理内存的释放
    auto scene = Scene::create();
    
    // 创建层对象，该对象将会由自动释放池管理内存的释放
    auto layer = LevelManager::create();
    
    // 将LevelManager层作为子节点添加到场景
    scene->addChild(layer);
    
    // 返回场景对象
    return scene;
}

//  "init" 函数中初始化实例
bool LevelManager::init()
{
    // 调用父类的init函数
    if ( !Layer::init() )
    {
        return false;
    }
    
    // 获得设备支持的可见OpenGL视图大小（即窗口大小）。
    Size visibleSize = Director::getInstance()->getVisibleSize();
    // 获得可见OpenGL视图的起源点（默认x=0，y=0）。
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 添加一个关闭菜单，点击可以退出程序
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(LevelManager::menuCloseCallback, this));
    // 设置关闭图标在屏幕的位置
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                 origin.y + closeItem->getContentSize().height/2));
    
    // 创建一个菜单项（Menu），该对象会自动管理内存的释放
    auto menu = Menu::create(closeItem, NULL);
    // 设置菜单项在屏幕的位置
    menu->setPosition(Vec2::ZERO);
    // 将菜单项添加到当前层的子节点
    this->addChild(menu, 1);
    
    // 默认选择第1关，索引下标从0开始
    _selectLevelIndex = 0;
    
    //创建分页层容器PageView
    auto pageView = PageView::create();
    // 设置PageView的ContentSize
    pageView->setContentSize(Size(480.0f, 320.0f));
    // 设置PageView在屏幕居中位置
    pageView->setPosition(Vec2((visibleSize.width - pageView->getContentSize().width) / 2.0f,
                               (visibleSize.height - pageView->getContentSize().height)*0.6));
    // 循环添加3个Layout（关卡图片）
    for (int i = 0; i < 3; ++i)
    {
        // 创建一个Layout
        Layout* layout = Layout::create();
        // 设置Layout的ContentSize
        layout->setContentSize(Size(480.0f, 320.0f));
        // 创建一个ImageView
        ImageView* imageView = ImageView::create(StringUtils::format("preview%d.png",i+1));
        imageView->setContentSize(Size(480.0f, 320.0f));
        imageView->setPosition(Vec2(layout->getContentSize().width / 2.0f, layout->getContentSize().height / 2.0f));
        // 将ImageView添加为Layout的子节点
        layout->addChild(imageView);
        // 在i表示的位置上插入Layout
        pageView->insertPage(layout,i);
    }
    // 添加事件监听器
    pageView->addEventListener([=](Ref* pSender,PageView::EventType type){
        switch (type)
        {
            case PageView::EventType::TURNING:
            {
                PageView* pageView = dynamic_cast<PageView*>(pSender);
                // CurPageIndex对应的就是当前关卡的索引
                _selectLevelIndex = pageView->getCurPageIndex();
            }
                break;
                
            default:
                break;
        }
    });
    // 将PageView添加未当层的子节点
    this->addChild(pageView,1);
    
    // “开始游戏”按钮
    auto start_button = Button::create("btn-start.png");
    start_button->setPosition(Vec2(visibleSize.width/2, visibleSize.height*0.2));
    start_button->addTouchEventListener([=](Ref* pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            // 右边滑入剧场效果
            auto transition = TransitionSlideInR::create(2.0, Game::createSceneWithLevel(++_selectLevelIndex));
            // push游戏场景
            Director::getInstance()->pushScene(transition);
        }
    });
    this->addChild(start_button,1);
    
    return true;
}


void LevelManager::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    // 程序执行结束,释放运行场景。
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
