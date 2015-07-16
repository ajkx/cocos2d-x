#ifndef __AngryBird__LevelScene
#define __AngryBird__LevelScene

#include "cocos2d.h"
#include "StartScene.h"
#include "GameScene.h"
#include "utils\LevelUtils.h"

USING_NS_CC;

class Level : public cocos2d::Layer
{
private:
	//当前成功次数
	int successLevel;
	int screenWidth,screenHeight;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Level);
};

#endif // __AngryBird__LevelScene