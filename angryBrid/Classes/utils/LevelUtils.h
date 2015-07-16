#ifndef __AngryBird__LevelUtils
#define __AngryBird__LevelUtils

#include "cocos2d.h"

USING_NS_CC;
class LevelUtils : public Ref
{
public:
	//读取关卡文件
	static int readLevelFromFile();
	//保存
	static void writeLevelToFile(int level);
};

#endif //__AngryBird__LevelUtils