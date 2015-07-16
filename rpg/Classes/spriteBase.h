#ifndef ___myRPG_SpriteBase__
#define ___myRPG_SpriteBase__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

class SpriteBase : public cocos2d::Sprite
{
private:
	int lifeValue; //精灵的生命值
	LoadingBar* HP; //精灵血条
	float HPInterval; //血条更新大小
	bool move;  //是否移动
	int moveCount; //移动次数    
public:
	static SpriteBase* create(const std::string& filename)
	{
		SpriteBase *sprite = new SpriteBase();
		if(sprite && sprite->initWithFile(filename)){
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	void setLifeValue(int _lifeValue){ lifeValue = _lifeValue;}
	int getLifeValue(){ return lifeValue;}

	void setHP(LoadingBar* _HP){ HP = _HP;}
	LoadingBar* getHP(){ return HP ;}

	void setHPInterval( float _HPInterval){ HPInterval = _HPInterval;}
	int getHPInterval(){ return HPInterval;}

	void setMove(bool _move){ move = _move;}
	bool isMove(){ return move;}

	void setMoveCount(int _moveCount){ moveCount = _moveCount;}
	int getMoveCount(){ return moveCount;}

};

#endif // ___myRPG_SpriteBase__
