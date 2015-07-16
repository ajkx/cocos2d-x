#ifndef __ProtectPrincess__GameMenuScene__
#define __ProtectPrincess__GameMenuScene__

#include "cocos2d.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

// 游戏的精灵表单、背景音乐和音效
#define RESOURCESPLIST "resources.plist"
#define MUSIC_FILE        "background.mp3"
#define EFFECT_FILE1        "Fat141.mp3"
#define EFFECT_FILE2       "Land232.mp3"

USING_NS_CC;;

class GameMenu : public cocos2d::Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameMenu);
};

#endif // __ProtectPrincess__GameMenuScene__