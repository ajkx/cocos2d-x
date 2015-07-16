#ifndef __AngryBird__GameScene
#define __AngryBird__GameScene

#include "cocos2d.h"
#include "LevelScene.h"
#include "utils\JsonParser.h"
#include "utils\LevelUtils.h"
#include "sprites\SpriteBase.h"
#include "sprites\Brid.h"
#include "sprites\Ice.h"
#include "sprites\Pig.h"
#include "sprites\SlingShot.h"

USING_NS_CC;

class Game : public cocos2d::Layer
{
protected:
	//小鸟集合
	Vector<Bird*> birdVector;
	//当前小鸟
	Bird* currentBird;
	//游戏的开始
	bool isStart;
	//游戏的结束
	bool isFinish;
	//小鸟是否碰撞标记
	bool isCollision;
	//小鸟的触摸状态
	int touchStatus;
	int screenWidth,screenHeight;
	//弹弓
	SlingShot* slingShot;	
	DrawNode* drawNode;
public:
	static cocos2d::Scene* createSceneWithLevel(int selectLevel);
	virtual bool init();
	void createLevel();
	//小鸟跳到弹弓上
	void jump();
	//计算出一个矩形区域代表精灵的可碰撞区域
	Rect rectOfSprite(Sprite* sprite);
	//小鸟碰撞后播放动画
	void playHitAnimation(float x,float y);
	
	void update(float delay);
	//场景内是否还有小猪
	bool hasPig();
	//场景内是否还有小鸟
	bool hasBird();
	//游戏结束
	void gameOver(std::string message);

	virtual void draw(Renderer *renderer,const Mat4& transform,uint32_t flag2)override;

	/*在碰撞刚发生时，onContactBegin会被调用，并且在此次碰撞中只会被调用一次。
	我们可以通过返回true或者false来决定物体是否发生碰撞。我们可以通过PhysicsContact::setData()
	来保存自己的数据以便用于后续的碰撞处理。需要注意的是，当onContactBegin返回flase时，
	onContactPreSolve和onContactPostSolve将不会被调用，但onContactSeperate必定会被调用。
     */
    bool onContactBegin(PhysicsContact& contact);
    
    /**
    onContactPreSolve发生在碰撞的每个步骤，我们你可以通过调用PhysicsContactPreSolve的
	设置函数来改变碰撞处理的一些参数设定，比如弹力，阻力等。同样可以通过返回true或者false
	来决定物体是否发生碰撞。还可以通过调用PhysicsContactPreSolve::ignore()来跳过后续的
	onContactPreSolve和onContactPostSolve回调事件通知（默认返回true）。
     */
    bool onContactPreSolve(PhysicsContact& contact, PhysicsContactPreSolve& solve);
    /**
     onContactPostSolve发生在碰撞计算完毕的每个步骤，你可以在此做一些碰撞的后续处理，
	 比如摧毁某个物体等。
     */
    void onContactPostSolve(PhysicsContact& contact, const PhysicsContactPostSolve& solve);
    /**
     onContactSeperate发生在碰撞结束两物体分离时，同样只会被调用一次。它跟onContactBegin必定
	 是成对出现的，所以可以在此摧毁之前通过PhysicsContact::setData()设置的用户数据。
     */
    void onContactSeperate(PhysicsContact& contact);

	CREATE_FUNC(Game);
};

#endif //__AngryBird__GameScene