#include "GameScene.h"

Scene* Game::createScene()
{
	auto scene = Scene::create();
	auto layer = Game::create();
	scene->addChild(layer);
	return scene;
}

bool Game::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto listener = EventListenerTouchOneByOne::create();
	score = 0;

	auto labelStr = Label::createWithSystemFont("score","HirakakuProN-W6",80);
	labelStr->setPosition(Vec2(visibleSize.width/5,visibleSize.height - 30));
	addChild(labelStr,5);

	labScore = Label::createWithSystemFont("0","HirakakuProN-W6",80);
	labScore->setPosition(Vec2(visibleSize.width/2+100,visibleSize.height - 30));
	addChild(labScore,5);

	auto layercolorBG = LayerColor::create(Color4B(180,170,160,160));
	this->addChild(layercolorBG,-1);

	listener->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan,this);
	listener->onTouchEnded = CC_CALLBACK_2(Game::onTouchEnded,this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	
	createCardSprite(visibleSize);
	autoCreateCard();
	autoCreateCard();
	return true;
}

void Game::createCardSprite(Size size)
{
	int lon = (size.width - 28) / 4;

	for(int i = 0; i < 4 ; i++){
		for(int j = 0 ; j < 4 ; j++){
			CardSprite *card = CardSprite::createCardSprite(0,lon,lon,lon*i+20,lon*j+20);
			cardArray[i][j] = card;
			addChild(card);
		}

	}

}

bool Game::onTouchBegan(Touch* touch,Event* event)
{
	Point touchPos = touch->getLocation();
	firstX = touchPos.x;
	firstY = touchPos.y;
	return true;
}

void Game::onTouchEnded(Touch* touch,Event* event)
{
	Point touchPos = touch->getLocation();
	endX = firstX - touchPos.x;
	endY = firstY - touchPos.y;

	if(abs(endX) > abs(endY)){
		if(endX+5>0){
			if(doLeft()){
				autoCreateCard();
				checkGame();
			}

		}else{
			if(doRight()){
				autoCreateCard();
				checkGame();
			}
		}
	}else{
		if(endY+5>0){
			if(doDown()){
				autoCreateCard();
				checkGame();
			}
		}else{
			if(doUp()){
				autoCreateCard();
				checkGame();
			}
		}
	}
}


bool Game::doLeft(){
	bool isdo = false;
		for(int y = 0; y < 4 ; y++){
			for(int x = 0 ; x < 4 ; x++){
				for(int x1 = x+1;x1 < 4; x1++){
					if(cardArray[x1][y]->getNumber() > 0){
						if(cardArray[x][y]->getNumber() <= 0){
							cardArray[x][y]->setNumber(cardArray[x1][y]->getNumber());
							cardArray[x1][y]->setNumber(0);
							x--;
							isdo = true;
						}else if(cardArray[x][y]->getNumber() == cardArray[x1][y]->getNumber()){
							cardArray[x][y]->setNumber(cardArray[x][y]->getNumber()*2);
							cardArray[x1][y]->setNumber(0);

							score+=cardArray[x][y]->getNumber();
							labScore->setString(__String::createWithFormat("%i",score)->getCString());

							isdo = true;
						}
						break;
					}
				}
			}
	}
	return isdo;
}


bool Game::doRight(){
	bool isdo = false;
		for(int y = 0; y < 4 ; y++){
			for(int x = 3 ; x >= 0 ; x--){
				for(int x1 = x-1;x1 >= 0; x1--){
					if(cardArray[x1][y]->getNumber() > 0){
						if(cardArray[x][y]->getNumber() <= 0){
							cardArray[x][y]->setNumber(cardArray[x1][y]->getNumber());
							cardArray[x1][y]->setNumber(0);
							x++;
							isdo = true;
						}else if(cardArray[x][y]->getNumber() == cardArray[x1][y]->getNumber()){
							cardArray[x][y]->setNumber(cardArray[x][y]->getNumber()*2);
							cardArray[x1][y]->setNumber(0);

							score+=cardArray[x][y]->getNumber();
							labScore->setString(__String::createWithFormat("%i",score)->getCString());

							isdo = true;
						}
						break;
					}
				}
			}
	}
	return isdo;
}


bool Game::doDown(){
	bool isdo = false;
		for(int x = 0; x < 4 ; x++){
			for(int y = 0 ; y < 4 ; y++){
				for(int y1 = y+1;y1 < 4; y1++){
					if(cardArray[x][y1]->getNumber() > 0){
						if(cardArray[x][y]->getNumber() <= 0){
							cardArray[x][y]->setNumber(cardArray[x][y1]->getNumber());
							cardArray[x][y1]->setNumber(0);
							y--;
							isdo = true;
						}else if(cardArray[x][y]->getNumber() == cardArray[x][y1]->getNumber()){
							cardArray[x][y]->setNumber(cardArray[x][y]->getNumber()*2);
							cardArray[x][y1]->setNumber(0);
							
							score+=cardArray[x][y]->getNumber();
							labScore->setString(__String::createWithFormat("%i",score)->getCString());
							isdo = true;
						}
						break;
					}
				}
			}
	}
	return isdo;
}


bool Game::doUp(){
	bool isdo = false;
		for(int x = 0; x < 4 ; x++){
			for(int y = 3 ; y >= 0 ; y--){
				for(int y1 = y-1;y1 >= 0; y1--){
					if(cardArray[x][y1]->getNumber() > 0){
						if(cardArray[x][y]->getNumber() <= 0){
							cardArray[x][y]->setNumber(cardArray[x][y1]->getNumber());
							cardArray[x][y1]->setNumber(0);
							y++;
							isdo = true;
						}else if(cardArray[x][y]->getNumber() == cardArray[x][y1]->getNumber()){
							cardArray[x][y]->setNumber(cardArray[x][y]->getNumber()*2);
							cardArray[x][y1]->setNumber(0);
							
							score+=cardArray[x][y]->getNumber();
							labScore->setString(__String::createWithFormat("%i",score)->getCString());
							isdo = true;
						}
						break;
					}
				}
			}
	}
	return isdo;
}

void Game::autoCreateCard()
{
	int i = CCRANDOM_0_1()*4;
	int j = CCRANDOM_0_1()*4;

	//ÅÐ¶ÏÊÇ·ñÎª¿Õ
	if(cardArray[i][j]->getNumber() > 0){
		autoCreateCard();
	}else{
		cardArray[i][j]->setNumber(CCRANDOM_0_1()*10 < 1 ? 4 : 2);
	}
}

void Game::checkGame()
{
	bool isOver = true;
	for( int  y = 0; y < 4;y++){
		for(int x=0;x<4;x++){
			if(cardArray[x][y]->getNumber() == 0 ||
				(x > 0 && (cardArray[x][y]->getNumber() == cardArray[x-1][y]->getNumber()))||
				(x < 3 && (cardArray[x][y]->getNumber() == cardArray[x+1][y]->getNumber()))||
				(y > 0 && (cardArray[x][y]->getNumber() == cardArray[x][y-1]->getNumber()))||
				(y < 0 && (cardArray[x][y]->getNumber() == cardArray[x][y+1]->getNumber()))){
					isOver = false;
			}
		}
	}
	if(isOver){
		Director::getInstance()->replaceScene(TransitionFade::create(1,Game::createScene()));
	}

}
