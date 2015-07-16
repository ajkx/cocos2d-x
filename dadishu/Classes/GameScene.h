#ifndef __WhackAMoleGame__GameScene__
#define __WhackAMoleGame__GameScene__

#include <iostream>
#include "cocos2d.h"
#include "StartScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
using namespace CocosDenshion;

USING_NS_CC;
using namespace ui;

static int level = 1;

class Game : public cocos2d::Layer
{
private:
	int screenWidth,screenHeight;
	Vector<Sprite*> molesVector;
	Label* levelLabel;
	Label* passLabel; // ¹ý¹Ø·ÖÊýµÄLabel
    Label* scoreLabel; // ÏÔÊ¾·ÖÊýµÄLabel
    Label* moleLabel; // ÏÔÊ¾µØÊó×ÜÊýµÄLabel
    int score;  // ·ÖÊý

    int sucessScore; // ¹ý¹ØÄ¿±ê·ÖÊý
    int totalMoles; // µ±Ç°¹Ø¿¨×Ü¹²×ê³öµØÊóÊýÁ¿
    LoadingBar* timeBar; // ¼ÆÊ±Æ÷½ø¶ÈÌõ
    float timeBarInterval; // Ê±¼äÌõ¼ä¸ô
    
    float popDelay; // Ëæ»úµ¯³öµØÊóµÄÊ±¼ä
    bool _gameOver; // ÓÎÏ·ÊÇ·ñ½áÊø
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	
	void andomPopMoles(float delte);

	void popMole(Sprite* mole);

	void setHit(Ref* pSender);

	void unHit(Ref* pSender);

	void gameOver(float delta);

	CREATE_FUNC(Game);
};
#endif /* defined(__WhackAMole__GameScene__) */