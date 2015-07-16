#include "GameMenuScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

// ��Ϸ�˵���
Scene* GameMenu::createScene()
{
    // ����һ���������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
    auto scene = Scene::create();
    
    // ��������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
    auto layer = GameMenu::create();
    
    // ��GameMenu����Ϊ�ӽڵ���ӵ�����
    scene->addChild(layer);
    
    // ���س�������
    return scene;
}

bool GameMenu::init()
{
	if ( !Layer::init() )
    {
        return false;
    }

	// ����豸֧�ֵĿɼ�OpenGL��ͼ��С�������ڴ�С����
    Size visibleSize = Director::getInstance()->getVisibleSize();
    // ��ÿɼ�OpenGL��ͼ����Դ�㣨Ĭ��x=0��y=0����
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(RESOURCESPLIST);
	SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_FILE1);
    SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_FILE2);
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(MUSIC_FILE);


	// ����ʼ��Ϸ����ť
    auto start_button = Button::create("btn-start.png");
    start_button->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2+75));
    // ����ʼ��Ϸ����ť��Ӵ�������
    start_button->addTouchEventListener([=](Ref* pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            // ���һ����糡Ч��
            auto transition = TransitionSplitRows::create(2.0, Game::createSceneWithLevel(1));
            // push��Ϸ����
            Director::getInstance()->pushScene(transition);
        }
    });
    this->addChild(start_button,1);

	// ��ѡ��ؿ�����ť
    auto select_button = Button::create("btn-select.png");
    select_button->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2-75));
    // ��ѡ��ؿ�����ť��Ӵ�������
    select_button->addTouchEventListener([=](Ref* pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            // �ұ߻���糡Ч��
            auto transition = TransitionSlideInR::create(2.0, LevelManager::createScene());
            // push��Ϸ����
            Director::getInstance()->pushScene(transition);
        }
    });
    this->addChild(select_button,1);
	return true;
}
