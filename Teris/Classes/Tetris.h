#ifndef __AITetris__Tetris__
#define __AITetris__Tetris__

#include "cocos2d.h"

//��������
typedef enum {
	BrickType_NULL = -1, //��
	BrickType_o,	//������
	BrickType_T,	// T��
	BrickType_7,	//7����
	BrickType_r7,	//��7
	BrickType_s,
	BrickType_z,
	BrickType_1,	//����
	BrickType_Count,
}BrickType;
//���鵥Ԫֵ
typedef enum{
	BrickCellValue_NULL,
	BrickCellValue_1,
	BrickCellValue_2,
	BrickCellValue_3,
	BrickCellValue_4,
	BrickCellValue_5,
	BrickCellValue_6,
	BrickCellValue_7,
	BrickCellValue_Reserve,
}BrickCellValue;

class Tetris
{
public:
	Tetris(int row,int column);
	~Tetris();
	// ����������
	int getRow(){ return m_row;}
	int getColumn(){ return m_column;}
	// ָ���кź��кŵķ��鵥Ԫֵ
	BrickCellValue getBrickValue(int r,int c);
	//��շ������
	void clearBrickMatrix();
	//�����·���
	bool setNewBrick(BrickType brickType,BrickCellValue brickCellValue);

	//�������
	bool moveLeftBrick();
	bool moveRightBrick();
	bool rotateBrick();
	bool fallBrick();
	void throwDownBrick();
	bool clearOneLine(int &lineIndex);

	//�������
	BrickCellValue *getBrickMatrix(){ return m_brickMatrix;}
	BrickCellValue (*getCurrentBrick())[4] { return m_currentBrick;}
	BrickType getCurrentBrickType(){ return m_currentBrickType;}
	int getPosRowOfCurrentBrick(){ return m_posRow;}
	int getPosColOfCurrentBrick(){ return m_posCol;}
	static bool setNewBrick(BrickCellValue *brickMatrix,int row,int column,
							BrickCellValue currentBrick[4][4],BrickType brickType,BrickCellValue brickCellValue,
							int &posRow,int&posCol);
	static bool rotateBrick(BrickCellValue *brickMatrix,int row,int column,
							BrickCellValue currentBrick[4][4], BrickType currentBrickType, int posRow, int posCol);
	static bool moveLeftBrick(BrickCellValue *brickMatrix,int row,int column,
							  BrickCellValue currentBrick[4][4],int posRow,int &posCol);
	static bool moveRightBrick(BrickCellValue *brickMatrix, int row, int column,
                               BrickCellValue currentBrick[4][4], int posRow, int &posCol);
	static bool fallBrick(BrickCellValue *brickMatrix, int row, int column,
                          BrickCellValue currentBrick[4][4], int &posRow, int posCol);
    static void throwDownBrick(BrickCellValue *brickMatrix, int row, int column,
                               BrickCellValue currentBrick[4][4], int &posRow, int posCol);
    static void fixBrick(BrickCellValue *brickMatrix, int row, int column,
                         BrickCellValue currentBrick[4][4], int posRow, int posCol);
    static bool clearOneLine(BrickCellValue *brickMatrix, int row, int column,
                             int posRowCurrentBrick, int &lineIndex);
public:
	//������Ϸ������С��Χ
	static const int MinRow = 8;
	static const int MinColumn = 4;
private:
	// ����ģ�� 8��
	static unsigned char m_BrickMode[BrickType_Count];
	//��Ϸ�����������������������
	int m_row,m_column;
	BrickCellValue *m_brickMatrix;
	//��ǰ�����λ��
	int m_posRow,m_posCol;
	//��ǰ����
	BrickType m_currentBrickType;
	BrickCellValue m_currentBrick[4][4];
};


#endif /* defined(__AITetris__Tetris__) */