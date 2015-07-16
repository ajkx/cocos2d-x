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
	//С�񼯺�
	Vector<Bird*> birdVector;
	//��ǰС��
	Bird* currentBird;
	//��Ϸ�Ŀ�ʼ
	bool isStart;
	//��Ϸ�Ľ���
	bool isFinish;
	//С���Ƿ���ײ���
	bool isCollision;
	//С��Ĵ���״̬
	int touchStatus;
	int screenWidth,screenHeight;
	//����
	SlingShot* slingShot;	
	DrawNode* drawNode;
public:
	static cocos2d::Scene* createSceneWithLevel(int selectLevel);
	virtual bool init();
	void createLevel();
	//С������������
	void jump();
	//�����һ���������������Ŀ���ײ����
	Rect rectOfSprite(Sprite* sprite);
	//С����ײ�󲥷Ŷ���
	void playHitAnimation(float x,float y);
	
	void update(float delay);
	//�������Ƿ���С��
	bool hasPig();
	//�������Ƿ���С��
	bool hasBird();
	//��Ϸ����
	void gameOver(std::string message);

	virtual void draw(Renderer *renderer,const Mat4& transform,uint32_t flag2)override;

	/*����ײ�շ���ʱ��onContactBegin�ᱻ���ã������ڴ˴���ײ��ֻ�ᱻ����һ�Ρ�
	���ǿ���ͨ������true����false�����������Ƿ�����ײ�����ǿ���ͨ��PhysicsContact::setData()
	�������Լ��������Ա����ں�������ײ������Ҫע����ǣ���onContactBegin����flaseʱ��
	onContactPreSolve��onContactPostSolve�����ᱻ���ã���onContactSeperate�ض��ᱻ���á�
     */
    bool onContactBegin(PhysicsContact& contact);
    
    /**
    onContactPreSolve��������ײ��ÿ�����裬���������ͨ������PhysicsContactPreSolve��
	���ú������ı���ײ�����һЩ�����趨�����絯���������ȡ�ͬ������ͨ������true����false
	�����������Ƿ�����ײ��������ͨ������PhysicsContactPreSolve::ignore()������������
	onContactPreSolve��onContactPostSolve�ص��¼�֪ͨ��Ĭ�Ϸ���true����
     */
    bool onContactPreSolve(PhysicsContact& contact, PhysicsContactPreSolve& solve);
    /**
     onContactPostSolve��������ײ������ϵ�ÿ�����裬������ڴ���һЩ��ײ�ĺ�������
	 ����ݻ�ĳ������ȡ�
     */
    void onContactPostSolve(PhysicsContact& contact, const PhysicsContactPostSolve& solve);
    /**
     onContactSeperate��������ײ�������������ʱ��ͬ��ֻ�ᱻ����һ�Ρ�����onContactBegin�ض�
	 �ǳɶԳ��ֵģ����Կ����ڴ˴ݻ�֮ǰͨ��PhysicsContact::setData()���õ��û����ݡ�
     */
    void onContactSeperate(PhysicsContact& contact);

	CREATE_FUNC(Game);
};

#endif //__AngryBird__GameScene