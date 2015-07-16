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
	//����С��
	void createJumpBrid();
	//С����Ծ
	void birdJump(float delta);
	//С����ɶ������������Ч��
	void birdExplosition(Ref* pSender);
	//��ʼ��Ϸ
	void startGame();

	CREATE_FUNC(Start);
};

#endif //__AngryBird__StartScene__