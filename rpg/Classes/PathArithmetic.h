#ifndef __myRPG_PathArithmetic
#define __myRPG_PathArithmetic

#include <vector>
#include "cocos2d.h"
#include "Grid.h"
#include "PointDelegate.h"
USING_NS_CC;
using namespace std;
enum{
	mapWidth = 26,
	mapHeight = 20
};

class PathArithmetic :public cocos2d::Ref
{
private:
	//记录已经过的点  F = G + H   为G（n）点
	Vector<PointDelegate*> invalidPoints; //G
	Vector<PointDelegate*> pathPoints;   ///H
public:
	static PathArithmetic* create(){
		PathArithmetic* p = new PathArithmetic();
		if(p){
			p->autorelease();
		}else{
			CC_SAFE_DELETE(p);
		}
		return p;
	}

	Vector<PointDelegate*> getPath(Vec2 from,Vec2 to,std::vector<Vector<Grid*>> gridVector);

	bool findValidGrid(Vec2 from,Vec2 to ,std::vector<Vector<Grid*>> gridVector);
	bool isCheck(Vec2 point,std::vector<Vector<Grid*>> gridVector);
};

#endif // __myRPG_PathArithmetic
