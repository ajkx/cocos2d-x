#ifndef StartScene__
#define StartScene__

#include "cocos2d.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

#define BACKGROUNDPLIST "background.plist"
#define RESOURCESPLIST "resources.plist"
#define LAUGHEFFECT "laugh.caf"
#define OWEFFECT "ow.caf"
#define SECONDEFFECT "second.mp3"
#define BACKGROUNDMUSIC "whack.caf"

class Start : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void startGame();
	Animation* getAnimationByName(std::string animName,float delay,int animNum);
	void menuCloseCallback(Ref* pSender);
	CREATE_FUNC(Start);
};
#endif /* defined(StartScene__) */
