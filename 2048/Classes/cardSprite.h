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

	void setNumber(int num);//�������ֵĴ�С
	int getNumber();
private:
	//��ʾ������
	int number;
	void enemyInit(int numbers,int width,int height,float CardSpriteX,float CardSpriteY);

	//��ʾ���ֵĿؼ�
	Label *labCardNumber;

	//��ʾ����
	LayerColor *layerColorBG;
};



#endif // __2048game_cardSprite