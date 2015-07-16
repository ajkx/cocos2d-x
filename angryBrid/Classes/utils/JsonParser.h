#ifndef __AngryBird__JsonParser
#define __AngryBird__JsonParser

#include "cocos2d.h"
#include "SpriteData.h"
#include "json\document.h"

USING_NS_CC;

class JsonParser : public Ref
{
public:
	static JsonParser* create(){
		JsonParser *sprite = new JsonParser();
		if(sprite){
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	// 解析JSON文件获取所需的精灵数据
	Vector<SpriteData*> getAllSpriteWithFile(std::string fileName);
};

#endif //__AngryBird__JsonParser