#include "LevelUtils.h"

int LevelUtils::readLevelFromFile()
{
	int successLevel = UserDefault::getInstance()->getIntegerForKey("SuccessLevel");
	if(successLevel){
		return successLevel;
	}
	
	return 0;
}


void LevelUtils::writeLevelToFile(int level){
	UserDefault::getInstance()->setIntegerForKey("SuccessLevel",level);
}
