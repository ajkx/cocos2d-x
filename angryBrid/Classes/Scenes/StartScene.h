#ifndef __AngryBird__StartScene__
#define __AngryBird__StartScene__

#include "cocos2d.h"
#include "PreLoadScene.h"
#include "LevelScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

USING_NS_CC;

class Start : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	//创建小鸟
	void createJumpBrid();
	//小鸟跳跃
	void birdJump(float delta);
	//小鸟完成动作后产生粒子效果
	void birdExplosition(Ref* pSender);
	//开始游戏
	void startGame();

	CREATE_FUNC(Start);
};

#endif //__AngryBird__StartScene__