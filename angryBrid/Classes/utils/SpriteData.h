#ifndef __AngryBird__SpriteData
#define __AngryBird__SpriteData

#include "cocos2d.h"

USING_NS_CC;

class SpriteData : public Ref
{
protected:
	int tag;
	int x;
	int y;
public:
	static SpriteData* create()
	{
		SpriteData *sprite = new SpriteData();
		if(sprite){
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	void setTag(int _tag){ tag = _tag;};
	int getTag(){ return tag;};

	    void setX(int _x){x = _x;};
    int getX(){return x;};
    
    void setY(int _y){ y = _y;};
    int getY(){return y;};

};

#endif //__AngryBird__SpriteData