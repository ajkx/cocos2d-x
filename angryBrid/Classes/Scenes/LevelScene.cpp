#include "LevelScene.h"

Scene* Level::createScene()
{
    // 创建一个场景对象，该对象将会由自动释放池管理内存的释放
    auto scene = Scene::create();
    
    // 创建层对象，该对象将会由自动释放池管理内存的释放
    auto layer = Level::create();
    
    // 将Preload层作为子节点添加到场景
    scene->addChild(layer);
    
    // 返回场景对象
    return scene;
}

bool Level::init()
{
	if ( !Layer::init() )
    {
        return false;
    }

	Size visibleSize = Director::getInstance()->getVisibleSize();
    // 给私有变量赋值，保存屏幕宽度和高度
    screenWidth = visibleSize.width;
    screenHeight = visibleSize.height;

	    // 背景图片
    auto selectlevel = Sprite::create("selectlevel.png");
    selectlevel->setPosition(screenWidth/2, screenHeight/2);
    this->addChild(selectlevel);
    // 返回图片
	auto back = Sprite::create("back.png");
    back->setPosition(40, 40);
    back->setScale(0.5f);
    // 设置tag用于标识
    back->setTag(100);
    this->addChild(back);

	successLevel = LevelUtils::readLevelFromFile();
	//显示12个关卡
	std::string imagePath = "";
	for(int i = 0; i < 12; i++){
		if(i < successLevel){
			imagePath = "level.png";
			std::string str = StringUtils::format("%d",i+1);
			auto *num = Label::createWithSystemFont(str,"Arial-BoldMT",40,Size(70,70),TextHAlignment::CENTER);

			//一排6个图标，所以i%6
			float x = 100+i%6*150;
			float y = screenHeight - 190-i/6*150;
			num->setPosition(x,y);
			this->addChild(num,2);
		}else{	
			imagePath = "clock.png";
		}

		//关卡精灵图片
		auto level = Sprite::create(imagePath);
		//设置TAG标识符
		level->setTag(i+1);
		float x = 100+i%6*150;
		float y = screenHeight-180-i/6*150;
		level->setPosition(x,y);
		level->setScale(0.5f);
		this->addChild(level,1);
	}
	
	auto gameListener = EventListenerTouchOneByOne::create();
	gameListener->onTouchBegan = [=](Touch* touch,Event* event){
	Vec2 touchLocation = touch->getLocation();
	Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);
	//判断点击哪个图片
	for(int i = 0; i < this->getChildrenCount();i++){
		auto tempSprite = this->getChildren().at(i);
		if(tempSprite->getBoundingBox().containsPoint(nodeLocation)){
			//返回按钮
			if(tempSprite->getTag() == 100){
				auto transition = TransitionSplitRows::create(2.0f,Start::createScene());
				Director::getInstance()->replaceScene(transition);
			}
			//已开放关卡
			else if((tempSprite->getTag() < successLevel+1) && tempSprite->getTag() > 0){
				Director::getInstance()->getEventDispatcher()->removeEventListener(gameListener);
				//
				auto transition = TransitionSplitRows::create(2.0f,Game::createSceneWithLevel(tempSprite->getTag()));
				Director::getInstance()->replaceScene(transition);
			}
			//如果玩家选择的是未开放的关卡，弹出提示
			else if(tempSprite->getTag() != -1){
				std::string message = StringUtils::format("第[%d]关未开放",tempSprite->getTag());
				MessageBox(message.c_str(),"提示");
			}
		}
	}
	return true;
};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(gameListener,this);
	return true;

}