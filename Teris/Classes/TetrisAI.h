#ifndef __AITetris__TetrisAI__
#define __AITetris__TetrisAI__

#include "Tetris.h"
typedef enum {
    OperationType_NULL,
    OperationType_Rotate,
    OperationType_Left,
    OperationType_Right,
    OperationType_ThrowDown,
}OperationType;

class TetrisAI
{
public:
	// currentBrickΪ����ָ�룬���鳤�ȹ̶�Ϊ4�����������ȱʡ��
    // ���������Ĳ��������Ϊ(3+column/2+1)
    static bool seekSolution(BrickCellValue *brickMatrix, int row, int column,
                             BrickCellValue (*currentBrick)[4], BrickType currentBrickType,
                             int posRow, int posCol, OperationType operationList[]);
private:
    // ���۲������
    static int evaluateBrickMatrix(BrickCellValue *brickMatrix, int row, int column,
                                   BrickCellValue (*currentBrick)[4], int posRow);
    // ���������ֵ
    static int landingHeight(int row, BrickCellValue (*currentBrick)[4], int posRow);
    static int erodedPieceCellsMetric(BrickCellValue *brickMatrix, int row, int column, int posRow);
    static int boardRowTransitions(BrickCellValue *brickMatrix, int row, int column);
    static int boardColTransitions(BrickCellValue *brickMatrix, int row, int column);
    static int boardBuriedHoles(BrickCellValue *brickMatrix, int row, int column);
    static int boardWells(BrickCellValue *brickMatrix, int row, int column);
};




#endif /* defined(__AITetris__TetrisAI__) */