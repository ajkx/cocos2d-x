#ifndef __AITetris__GameLayer__
#define __AITetris__GameLayer__

#include "Tetris.h"
#include "TetrisAI.h"
#include "cocos2d.h"

USING_NS_CC;

class GameLayer : public cocos2d::Layer
{
public:
	virtual bool init();
	CREATE_FUNC(GameLayer);

	void startGame();
	void startAI(bool AIOn);
	void accelerateAI();
	void decelerateAI();
	float getAISpeed(){ return 1/m_speedAI; }
	//�������
	void moveLeftBrick();
	void moveRightBrick();
	void rotateBrick();
	void fallBrick();
	void throwDownBrick();

private:
	// ˢ�·������
	void updateTetrisBrickMatrix();
	// ��log����ʾ��Ϸ
	void showTetrisInLog();
    // ִ��AIϵͳ���صĲ����б�
    void commitOperationList(int index);
	// Tetris�߼�
	Tetris *m_tetris;
	bool m_gameRun;
	// ���鷽�飬���ڷ�������չ��
	static const int Row_Tetris = 21;
	static const int Column_Tetris = 12;
	Sprite **m_spriteBrick;
	//AI����
	float m_speedAI;
	Action *m_actionAI;
	OperationType m_operationList[3+12/2+1];
};

#endif /* defined(__AITetris__GameLayer__) */