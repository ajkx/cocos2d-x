#ifndef __myRPG_PointDelegate
#define __myRPG_PointDelegate
#include "cocos2d.h"

class PointDelegate : public cocos2d::Ref
{
private :
	float x;
	float y;
public:
	static PointDelegate* create(float _x,float _y){
		PointDelegate* p = new PointDelegate();
		if(p && p->initPoint(_x,_y)){
			p->autorelease();
			return p;
		}
		CC_SAFE_DELETE(p);
		return nullptr;
	}
	bool initPoint(float _x,float _y){
		x = _x;
		y = _y;
		return true;
	}
	void setX(float _x){ x=_x;}
	float getX(){return x;}
	void setY(float _y){ y=_y;}
	float getY(){return y;}

};
#endif // __myRPG_PointDelegate