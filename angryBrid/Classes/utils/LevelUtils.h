#ifndef __AngryBird__LevelUtils
#define __AngryBird__LevelUtils

#include "cocos2d.h"

USING_NS_CC;
class LevelUtils : public Ref
{
public:
	//��ȡ�ؿ��ļ�
	static int readLevelFromFile();
	//����
	static void writeLevelToFile(int level);
};

#endif //__AngryBird__LevelUtils