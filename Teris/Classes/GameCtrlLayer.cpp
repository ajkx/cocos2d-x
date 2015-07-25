#include "GameCtrlLayer.h"

bool GameCtrlLayer::init()
{
	if(!Layer::init()){
		return false;
	}
	m_AIOn = false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	//��ʼ��Ϸ
	Label *labelStartGame = Label::createWithSystemFont("��ʼ��Ϸ","",24);
	labelStartGame->setWidth(36*3);
	labelStartGame->setAlignment(TextHAlignment::CENTER);
	MenuItemLabel *menuItemStartGame = MenuItemLabel::create(labelStartGame,
		CC_CALLBACK_0(GameCtrlLayer::clickStartGameButton,this));
	Menu *menuStartGame = Menu::createWithItem(menuItemStartGame);
	menuStartGame->setPosition(36+36/2,visibleSize.height/2-10);
	this->addChild(menuStartGame);
	//��������
	m_labelAISwitch = Label::createWithSystemFont("��������","",24);
	m_labelAISwitch->setWidth(36*3);
	m_labelAISwitch->setAlignment(TextHAlignment::CENTER);
	MenuItemLabel *menuItemAISwitch = MenuItemLabel::create(m_labelAISwitch, CC_CALLBACK_0(GameCtrlLayer::clickAISwitchButton, this));
	Menu *menuAISwitch = Menu::createWithItem(menuItemAISwitch);
    menuAISwitch->setPosition(36+36/2, visibleSize.height/2-60);
    this->addChild(menuAISwitch);
	// �����ٶ�
    m_labelAISpeed = Label::createWithSystemFont("", "", 24);
    m_labelAISpeed->setWidth(36*3);
    m_labelAISpeed->setAlignment(TextHAlignment::CENTER);
    m_labelAISpeed->setPosition(36+36/2, visibleSize.height/2-4*36);
    this->addChild(m_labelAISpeed);

// ��
    MenuItemImage *menuItemUp = MenuItemImage::create("btn_up.png", "btn_up.png", CC_CALLBACK_0(GameCtrlLayer::clickUpButton, this));
    Menu *menuUp = Menu::createWithItem(menuItemUp);
    menuUp->setPosition(36+36/2, visibleSize.height/2-3*36);
    this->addChild(menuUp);
    // ��
    MenuItemImage *menuItemLeft = MenuItemImage::create("btn_left.png", "btn_left.png", CC_CALLBACK_0(GameCtrlLayer::clickLeftButton, this));
    m_menuLeft = Menu::createWithItem(menuItemLeft);
    m_menuLeft->setPosition(36/2, visibleSize.height/2-4*36);
    this->addChild(m_menuLeft);
    // ��
    MenuItemImage *menuItemDown = MenuItemImage::create("btn_down.png", "btn_down.png", CC_CALLBACK_0(GameCtrlLayer::clickDownButton, this));
    Menu *menuDown = Menu::createWithItem(menuItemDown);
    menuDown->setPosition(36+36/2, visibleSize.height/2-5*36);
    this->addChild(menuDown);
    // ��
    MenuItemImage *menuItemRight = MenuItemImage::create("btn_right.png", "btn_right.png", CC_CALLBACK_0(GameCtrlLayer::clickRightButton, this));
    m_menuRight = Menu::createWithItem(menuItemRight);
    m_menuRight->setPosition(2*36+36/2, visibleSize.height/2-4*36);
    this->addChild(m_menuRight);
    return true;
}

void GameCtrlLayer::clickStartGameButton()
{
	m_delegate->gameCtrlLayerClickStartGame();
}
void GameCtrlLayer::clickAISwitchButton()
{
	m_AIOn = !m_AIOn;
	if(m_AIOn){
		m_labelAISwitch->setString("�ر�����");
		float speed = m_delegate->gameCtrlLayerGetAISpeed();
		m_labelAISpeed->setVisible(true);
		m_labelAISpeed->setString(StringUtils::format("%.1f��/��",speed));
		m_menuLeft->setVisible(false);
		m_menuRight->setVisible(false);
	}else{
		m_labelAISwitch->setString("��������");
        m_labelAISpeed->setVisible(false);
        m_menuLeft->setVisible(true);
        m_menuRight->setVisible(true);
	}
	m_delegate->gameCtrlLayerStartAI(m_AIOn);
}
void GameCtrlLayer::clickLeftButton()
{
	m_delegate->gameCtrlLayerClickLeft();
}
void GameCtrlLayer::clickRightButton()
{
    m_delegate->gameCtrlLayerClickRight();
}
void GameCtrlLayer::clickUpButton()
{
	if(m_AIOn){
		m_delegate->gameCtrlLayerClickAccelerate();
		float speed = m_delegate->gameCtrlLayerGetAISpeed();
		m_labelAISpeed->setString(StringUtils::format("%.1��/��",speed));
	}else{
		m_delegate->gameCtrlLayerClickRotate();
	}
}
void GameCtrlLayer::clickDownButton()
{
	if(m_AIOn){
		m_delegate->gameCtrlLayerClickDecelerate();
		float speed = m_delegate->gameCtrlLayerGetAISpeed();
		m_labelAISpeed->setString(StringUtils::format("%.1f��/��",speed));
	}else{
		m_delegate->gameCtrlLayerClickThrowDown();
	}
}
