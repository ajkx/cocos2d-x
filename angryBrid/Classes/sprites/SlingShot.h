#ifndef __AngryBird__SlingShot
#define __AngryBird__SlingShot

#include "cocos2d.h"

USING_NS_CC;

class SlingShot : public cocos2d::Sprite
{
protected:
	Vec2 startPoint1;
	Vec2 startPoint2;
	Vec2 endPoint;
public:
	static SlingShot* create()
	{
		SlingShot *sprite = new SlingShot();
        if (sprite && sprite->init())
        {
            sprite->autorelease();
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
	}

	void setStartPoint1(Vec2 _startPoint1){ startPoint1 = _startPoint1;};
	Vec2 getStartPoint1(){ return startPoint1; }

	void setStartPoint2(Vec2 _startPoint2){ startPoint2 = _startPoint2; }
    Vec2 getStartPoint2(){ return startPoint2; }
    
    void setEndPoint(Vec2 _endPoint){ endPoint = _endPoint; }
    Vec2 getEndPoint(){ return endPoint; }
};

#endif //__AngryBird__SlingShot