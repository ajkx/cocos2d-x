#include "cardSprite.h"

CardSprite *CardSprite::createCardSprite(int numbers,int width,int height,float CardSpriteX,float CardSpriteY)
{
	CardSprite *enemy = new CardSprite();
	if(enemy && enemy->init()){
		enemy->autorelease();
		enemy->enemyInit(numbers,width,height,CardSpriteX,CardSpriteY);
		return enemy;
	}
	CC_SAFE_DELETE(enemy);
	return nullptr;
}

bool CardSprite::init()
{
	if(!Sprite::init()){
		return false;
	}
	return true;
}

void CardSprite::setNumber(int num){
	number = num;
	if(number >= 0 ){
		labCardNumber->setSystemFontSize(100);
	}
	if(number >= 16 ){
		labCardNumber->setSystemFontSize(90);
	}
	if(number >= 128 ){
		labCardNumber->setSystemFontSize(60);
	}
	if(number >= 1024 ){
		labCardNumber->setSystemFontSize(40);
	}

	if(number == 0){
		layerColorBG->setColor(Color3B(200,190,180));
	}
	if(number == 2){
		layerColorBG->setColor(Color3B(240,230,220));
	}
	if(number == 4){
		layerColorBG->setColor(Color3B(240,220,200));
	}
	if(number == 8){
		layerColorBG->setColor(Color3B(240,180,120));
	}
	if(number == 16){
		layerColorBG->setColor(Color3B(240,140,90));
	}
	if(number == 32){
		layerColorBG->setColor(Color3B(240,90,60));
	}
	if(number == 64){
		layerColorBG->setColor(Color3B(240,200,70));
	}
	if(number == 128){
		layerColorBG->setColor(Color3B(240,150,60));
	}
	if(number == 256){
		layerColorBG->setColor(Color3B(240,150,60));
	}
	if(number ==512){
		layerColorBG->setColor(Color3B(240,190,80));
	}
	if(number == 1024){
		layerColorBG->setColor(Color3B(0,130,0));
	}
	if(number == 2048){
		layerColorBG->setColor(Color3B(0,160,0));
	}


	if(number > 0){
		labCardNumber->setString(__String::createWithFormat("%i",number)->getCString());
	}else{
		labCardNumber->setString("");
	}
}

int CardSprite::getNumber(){
	return number;
}

void CardSprite::enemyInit(int numbers,int width,int height,float CardSpriteX,float CardSpriteY){
	number = numbers;
	layerColorBG = LayerColor::create(Color4B(200,190,180,255),width - 15,height - 15);
	layerColorBG->setPosition(Vec2(CardSpriteX,CardSpriteY));
	if(number > 0){
		labCardNumber = Label::createWithSystemFont(__String::createWithFormat("%i",number)->getCString(),"HirakakuProN-W6",100);
		labCardNumber->setPosition(Vec2(layerColorBG->getContentSize().width/2,layerColorBG->getContentSize().height/2));
		layerColorBG->addChild(labCardNumber);
	}else{
		labCardNumber = Label::createWithSystemFont("","HirakakuProN-W6",100);
		labCardNumber->setPosition(Vec2(layerColorBG->getContentSize().width/2,layerColorBG->getContentSize().height/2));
		layerColorBG->addChild(labCardNumber);
	}
	this->addChild(layerColorBG);
}

