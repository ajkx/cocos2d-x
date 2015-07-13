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

	//自动生成卡片
	void autoCreateCard();
	//判断游戏是否结束
	void checkGame();
private:
	int firstX,firstY,endX,endY;
	CardSprite *cardArray[4][4];
	void createCardSprite(Size size);

	//分数
	int score;
	Label *labScore;
};

#endif // __2048game_GameScene