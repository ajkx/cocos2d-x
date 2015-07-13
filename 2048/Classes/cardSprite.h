#ifndef __2048game_cardSprite
#define __2048game_cardSprite
#include "cocos2d.h"
USING_NS_CC;

class CardSprite : public cocos2d::Sprite
{
public:
	static CardSprite *createCardSprite(int numbers,int width,int height,float CardSpriteX,float CardSpriteY);
	virtual bool init();
	CREATE_FUNC(CardSprite);

	void setNumber(int num);//设置数字的大小
	int getNumber();
private:
	//显示的数字
	int number;
	void enemyInit(int numbers,int width,int height,float CardSpriteX,float CardSpriteY);

	//显示数字的控件
	Label *labCardNumber;

	//显示背景
	LayerColor *layerColorBG;
};



#endif // __2048game_cardSprite