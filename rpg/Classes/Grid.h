#ifndef __myRPG_Grid
#define __myRPG_Grid
#include "cocos2d.h"		//网格类，代表 tile

class Grid : public cocos2d::Ref
{
public:
	static Grid* create(int x,int y){
		Grid* g = new Grid();
		if(g && g->initWithPoint(x,y)){
			g->autorelease();
			return g;
		}
		CC_SAFE_DELETE(g);
		return nullptr;
	}
	bool initWithPoint(int _x,int _y){
		x = _x;
		y = _y;
		pass = true;
		return true;
	}
	void setX(int _x){ x=_x;}
	int getX(){ return x; }
	void setY(int _y){ y = _y; }
    int getY(){ return y; }

	void setPass(bool _pass){  pass = _pass; }
    bool isPass(){ return pass; }
private:
	int x;
	int y;
	bool pass;
};

#endif //__myRPG_Grid