#ifndef __2048game_GameScene
#define __2048game_GameScene

#include "cocos2d.h"
#include "cardSprite.h"

USING_NS_CC;

class Game : public cocos2d::Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Game);

	virtual bool onTouchBegan(Touch* touch,Event* event);
	virtual void onTouchEnded(Touch* touch,Event* event);
	
	bool doLeft();
	bool doRight();
	bool doUp();
	bool doDown();

	//�Զ����ɿ�Ƭ
	void autoCreateCard();
	//�ж���Ϸ�Ƿ����
	void checkGame();
private:
	int firstX,firstY,endX,endY;
	CardSprite *cardArray[4][4];
	void createCardSprite(Size size);

	//����
	int score;
	Label *labScore;
};

#endif // __2048game_GameScene