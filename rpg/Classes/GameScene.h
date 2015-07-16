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
	TMXTiledMap* tileMap;  //��ͼ 
	Sprite* player; //��Ҿ���
	Vector<SpriteBase*> monsterVector; //������
	TMXLayer* collidable;  //�ϰ���
	int screenWidth , screenHeight;
	int count;  //��Ϸ֡����;
	vector<Vector<Grid*>> _gridVector;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void onEnter();
	Animate* getAnimationByName(std::string animName,float delay,int animNum);	//��ȡ����
	Vec2 tileCoordForPosition(Vec2 position);	//ת������
	float getPlayerMoveTime(Vec2 startPos,Vec2 endPos);	//��ȡ�ƶ�ʱ��
	void playerMover(Vec2 position);		//�ƶ�player
	void setViewPointCenter(Vec2 position);		//���þ���Ϊ�Ӿ�����
	void playerMoveWithWayPoints(Vec2 position,Vector<PointDelegate*> path);	//���·���ƶ�
	Point locationForTilePos(Vec2 pos);
	Vec2 randomPosition();			//�����������
	Vec2 randomMovePosition(SpriteBase* monster);
	void updateMonsterSprite(float delta);
	void monsterRepeatedlyMove(float delta);	//���ﲻ�����ƶ�
	void monsterMoveWithWayPoints(Vector<PointDelegate*> pathVector,SpriteBase* monster);
	void update(float delta);
	void collisionDetection(float delta);
	CREATE_FUNC(Game);
};

#endif  // __myRPG_GameScene__