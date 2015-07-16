#include "PathArithmetic.h"
#include <algorithm>


bool PathArithmetic::isCheck(Vec2 point,vector<Vector<Grid*>> _gridArray){
	if(point.x < 0 || point.y < 0){
		return false;
	}

	PointDelegate* g = PointDelegate::create(point.x,point.y);
	for( int i=0; i < invalidPoints.size();i++){
		PointDelegate* pp = invalidPoints.at(i);
		Vec2 t = Vec2(pp->getX(),pp->getY());
		if(point.equals(t)){
			return false;
		}
	}
	if(point.x >= mapWidth || point.y >= mapHeight){
		return false;
	}

	Vector<Grid*> tempX = _gridArray.at((int)g->getX());
	Grid* grid = tempX.at((int)g->getY());
	if(point.x >= 0 && point.y >= 0 && grid->isPass()){
		return true;
	}
	return false;
}

bool PathArithmetic::findValidGrid(Vec2 from,Vec2 to,vector<Vector<Grid*>> _gridArray){
	PointDelegate* fromDelegate = PointDelegate::create(from.x,from.y);
	invalidPoints.pushBack(fromDelegate);
	Vector<PointDelegate*> points;
	//先添加原点周围八个点
	points.pushBack(PointDelegate::create(from.x,from.y-1));
	points.pushBack(PointDelegate::create(from.x,from.y+1));
	points.pushBack(PointDelegate::create(from.x-1,from.y));
	points.pushBack(PointDelegate::create(from.x+1,from.y));
	points.pushBack(PointDelegate::create(from.x-1,from.y-1));
	points.pushBack(PointDelegate::create(from.x+1,from.y+1));
	points.pushBack(PointDelegate::create(from.x-1,from.y+1));
	points.pushBack(PointDelegate::create(from.x+1,from.y-1));
	//使用临时vector集合
	Vector<PointDelegate*> temp;
	for(int i = 0; i<points.size();i++){
		PointDelegate* pd = points.at(i);
		Vec2 p = Vec2(pd->getX(),pd->getY());
		if(p.equals(to)){
			pathPoints.pushBack(pd);
			return true;
		}
		//检查点的有效性，即是否是障碍物
		if(isCheck(p,_gridArray)){
			temp.pushBack(pd);
		}
	}


	std::sort(temp.begin(),temp.end(),
		[=](const Ref* obj1,const Ref* obj2){
			PointDelegate* p1 = (PointDelegate*)obj1;
			PointDelegate* p2 = (PointDelegate*)obj2;
			double r1 = sqrt((p1->getX() - to.x) * (p1->getX() - to.x) + (p1->getY() - to.y) * (p1->getY() - to.y));
			double r2 = sqrt((p2->getX() - to.x) * (p2->getX() - to.x) + (p2->getY() - to.y) * (p2->getY() - to.y));
			return r1 < r2 ? -1 : 0;  //负数为从小到大
	});
	for(int i = 0; i < temp.size();i++){ //
		PointDelegate* pd = temp.at(i);
		Vec2 p = Vec2(pd->getX(),pd->getY());
		bool flag = findValidGrid(p,to,_gridArray);
		if(flag){
			pathPoints.pushBack(pd);
			return true;
		}
	}
	return false;
}


Vector<PointDelegate*> PathArithmetic::getPath(Vec2 from,Vec2 to,vector<Vector<Grid*>> _gridArray){
	findValidGrid(from,to,_gridArray);
	pathPoints.reverse();
	return pathPoints;
}



