#ifndef __AITetris__Tetris__
#define __AITetris__Tetris__

#include "cocos2d.h"

//方块类型
typedef enum {
	BrickType_NULL = -1, //空
	BrickType_o,	//正方形
	BrickType_T,	// T型
	BrickType_7,	//7字型
	BrickType_r7,	//反7
	BrickType_s,
	BrickType_z,
	BrickType_1,	//条形
	BrickType_Count,
}BrickType;
//方块单元值
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
	// 行数和列数
	int getRow(){ return m_row;}
	int getColumn(){ return m_column;}
	// 指定行号和列号的方块单元值
	BrickCellValue getBrickValue(int r,int c);
	//清空方块矩阵
	void clearBrickMatrix();
	//设置新方块
	bool setNewBrick(BrickType brickType,BrickCellValue brickCellValue);

	//方块操作
	bool moveLeftBrick();
	bool moveRightBrick();
	bool rotateBrick();
	bool fallBrick();
	void throwDownBrick();
	bool clearOneLine(int &lineIndex);

	//方块操作
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
	//整个游戏区的最小范围
	static const int MinRow = 8;
	static const int MinColumn = 4;
private:
	// 方块模板 8个
	static unsigned char m_BrickMode[BrickType_Count];
	//游戏区的行数与列数，方块矩阵
	int m_row,m_column;
	BrickCellValue *m_brickMatrix;
	//当前方块的位置
	int m_posRow,m_posCol;
	//当前方块
	BrickType m_currentBrickType;
	BrickCellValue m_currentBrick[4][4];
};


#endif /* defined(__AITetris__Tetris__) */