
#ifndef __myRPG_FightScene
#define __myRPG_FightScene
#include "cocos2d.h"
#include "spriteBase.h"
#include "GameScene.h"
enum class ParticleType
{
	bloodSpurts,
	magic
};
class Fight : public cocos2d::Layer
{
private:
	SpriteBase* player;
	SpriteBase* monster;
	bool isShoot;
	int screenWidth,screenHeight;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void update(float delta);
	void playerAction();
	void shootFinish();
	void monsterAction();
	ParticleSystem* getParticleWithName(ParticleType type);
	Animate* getAnimateByName(std::string animName,float delay,int animNum);

	CREATE_FUNC(Fight);
};


#endif // __myRPG_FightScene