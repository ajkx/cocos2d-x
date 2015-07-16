#ifndef __myRPG_GameScene__
#define __myRPG_GameScene__

#include "cocos2d.h"
#include "spriteBase.h"
#include "Grid.h"
#include "PathArithmetic.h"
#include "PointDelegate.h"
#include "FightScene.h"

USING_NS_CC;

#define MUSIC_FILE        "background.mp3"
#define FIGHT_FILE        "fight.mp3"
#define SHOOT_FILE        "shoot.wav"
#define BOMB_FILE        "bomb.mp3"

class Game : public cocos2d::Layer
{
private:
	TMXTiledMap* tileMap;  //地图 
	Sprite* player; //玩家精灵
	Vector<SpriteBase*> monsterVector; //怪物组
	TMXLayer* collidable;  //障碍物
	int screenWidth , screenHeight;
	int count;  //游戏帧计数;
	vector<Vector<Grid*>> _gridVector;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void onEnter();
	Animate* getAnimationByName(std::string animName,float delay,int animNum);	//获取动画
	Vec2 tileCoordForPosition(Vec2 position);	//转换坐标
	float getPlayerMoveTime(Vec2 startPos,Vec2 endPos);	//获取移动时间
	void playerMover(Vec2 position);		//移动player
	void setViewPointCenter(Vec2 position);		//设置精灵为视觉中心
	void playerMoveWithWayPoints(Vec2 position,Vector<PointDelegate*> path);	//最短路径移动
	Point locationForTilePos(Vec2 pos);
	Vec2 randomPosition();			//随机怪物坐标
	Vec2 randomMovePosition(SpriteBase* monster);
	void updateMonsterSprite(float delta);
	void monsterRepeatedlyMove(float delta);	//怪物不规则移动
	void monsterMoveWithWayPoints(Vector<PointDelegate*> pathVector,SpriteBase* monster);
	void update(float delta);
	void collisionDetection(float delta);
	CREATE_FUNC(Game);
};

#endif  // __myRPG_GameScene__