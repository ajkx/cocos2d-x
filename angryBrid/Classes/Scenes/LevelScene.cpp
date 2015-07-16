#include "LevelScene.h"

Scene* Level::createScene()
{
    // ����һ���������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
    auto scene = Scene::create();
    
    // ��������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
    auto layer = Level::create();
    
    // ��Preload����Ϊ�ӽڵ���ӵ�����
    scene->addChild(layer);
    
    // ���س�������
    return scene;
}

bool Level::init()
{
	if ( !Layer::init() )
    {
        return false;
    }

	Size visibleSize = Director::getInstance()->getVisibleSize();
    // ��˽�б�����ֵ��������Ļ��Ⱥ͸߶�
    screenWidth = visibleSize.width;
    screenHeight = visibleSize.height;

	    // ����ͼƬ
    auto selectlevel = Sprite::create("selectlevel.png");
    selectlevel->setPosition(screenWidth/2, screenHeight/2);
    this->addChild(selectlevel);
    // ����ͼƬ
	auto back = Sprite::create("back.png");
    back->setPosition(40, 40);
    back->setScale(0.5f);
    // ����tag���ڱ�ʶ
    back->setTag(100);
    this->addChild(back);

	successLevel = LevelUtils::readLevelFromFile();
	//��ʾ12���ؿ�
	std::string imagePath = "";
	for(int i = 0; i < 12; i++){
		if(i < successLevel){
			imagePath = "level.png";
			std::string str = StringUtils::format("%d",i+1);
			auto *num = Label::createWithSystemFont(str,"Arial-BoldMT",40,Size(70,70),TextHAlignment::CENTER);

			//һ��6��ͼ�꣬����i%6
			float x = 100+i%6*150;
			float y = screenHeight - 190-i/6*150;
			num->setPosition(x,y);
			this->addChild(num,2);
		}else{	
			imagePath = "clock.png";
		}

		//�ؿ�����ͼƬ
		auto level = Sprite::create(imagePath);
		//����TAG��ʶ��
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
	//�жϵ���ĸ�ͼƬ
	for(int i = 0; i < this->getChildrenCount();i++){
		auto tempSprite = this->getChildren().at(i);
		if(tempSprite->getBoundingBox().containsPoint(nodeLocation)){
			//���ذ�ť
			if(tempSprite->getTag() == 100){
				auto transition = TransitionSplitRows::create(2.0f,Start::createScene());
				Director::getInstance()->replaceScene(transition);
			}
			//�ѿ��Źؿ�
			else if((tempSprite->getTag() < successLevel+1) && tempSprite->getTag() > 0){
				Director::getInstance()->getEventDispatcher()->removeEventListener(gameListener);
				//
				auto transition = TransitionSplitRows::create(2.0f,Game::createSceneWithLevel(tempSprite->getTag()));
				Director::getInstance()->replaceScene(transition);
			}
			//������ѡ�����δ���ŵĹؿ���������ʾ
			else if(tempSprite->getTag() != -1){
				std::string message = StringUtils::format("��[%d]��δ����",tempSprite->getTag());
				MessageBox(message.c_str(),"��ʾ");
			}
		}
	}
	return true;
};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(gameListener,this);
	return true;

}