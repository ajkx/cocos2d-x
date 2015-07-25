#include "TetrisAI.h"

// currentBrick为数组指针，数组长度固定为4，这个参数就缺省了
// 操作方案的步骤数最大为(3+column/2+1)
bool TetrisAI::seekSolution(BrickCellValue *brickMatrix, int row, int column,
                            BrickCellValue (*currentBrick)[4], BrickType currentBrickType,
                            int posRow, int posCol, OperationType operationList[])
{
    operationList[0] = OperationType_ThrowDown;
    // 申请空间用于执行操作方案
    int tempPosRow = posRow, tempPosCol = posCol;
    BrickCellValue *tempBrickMatrix = new BrickCellValue[row*column];
    BrickCellValue tempCurrentBrick[4][4];
    // 复制方块矩阵及当前方块
    memcpy(tempBrickMatrix, brickMatrix, row*column*sizeof(BrickCellValue));
    memcpy(tempCurrentBrick, currentBrick, 4*4*sizeof(BrickCellValue));
    // 设置当前方块失败则表示游戏结束，直接返回即可
    if (!Tetris::setNewBrick(tempBrickMatrix, row, column, tempCurrentBrick, currentBrickType, BrickCellValue_1, tempPosRow, tempPosCol)) {
        delete []tempBrickMatrix;
        return false;
    }
    // 当前方块有方块的单元值改为BrickCellValue_Reserve，以便统计消去的数量
    BrickCellValue reserveCurrentBrick[4][4];
    memcpy(reserveCurrentBrick, currentBrick, 4*4*sizeof(BrickCellValue));
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c <4; c++) {
            if (BrickCellValue_NULL != reserveCurrentBrick[r][c]) {
                reserveCurrentBrick[r][c] = BrickCellValue_Reserve;
            }
        }
    }
    
    // 每种方块可旋转的次数
    int rotateCount = 4;
    if (BrickType_o == currentBrickType) {
        rotateCount = 1;
    }
    else if (BrickType_1 == currentBrickType ||
             BrickType_s == currentBrickType ||
             BrickType_z == currentBrickType) {
        rotateCount = 2;
    }
    // 只申请一种方案的空间用于执行即可
    int operationCount = 3+column/2+1;
    OperationType *operationSolution = new OperationType[operationCount];
    // 只记住当前选中的方案的得分即可
    int scoreBestSolution = -10000;
    // 另外记一下操作步骤的数量，以便在评价分相同的情况下选操作步骤最少的
    int operationMinCount = operationCount;
    // 可以旋转的最大次数
    for (int rotate = 0; rotate < rotateCount; rotate++) {
        // 可以平移的最大次数
        for (int move = 0; move < column; move++) {
            // 还原方块矩阵、当前方块及位置
            memcpy(tempBrickMatrix, brickMatrix, row*column*sizeof(BrickCellValue));
            memcpy(tempCurrentBrick, reserveCurrentBrick, 4*4*sizeof(BrickCellValue));
            tempPosRow = posRow, tempPosCol = posCol;
            // 该方案的位置
            int posOperation = 0;
            // 旋转次数
            for (int r = 0; r < rotate; r++) {
                operationSolution[posOperation++] = OperationType_Rotate;
            }
            // 左移次数
            if (move < column/2-1) {
                for (int m = 0; m < (column/2-1)-move; m++) { // 左移最大为左5
                    operationSolution[posOperation++] = OperationType_Left;
                }
            }
            // 右移次数
            else if (move > column/2-1) {
                for (int m = 0; m < move-(column/2-1); m++) { // 右移最大为右6
                    operationSolution[posOperation++] = OperationType_Right;
                }
            }
            // 不移动
            else {
            }
            // 追加丢下操作
            operationSolution[posOperation++] = OperationType_ThrowDown;
            // 试试该方案是否可行
            bool valid = true;
            for (int i = 0; i < posOperation && valid; i++) {
                if (OperationType_Rotate == operationSolution[i]) {
                    if (!Tetris::rotateBrick(tempBrickMatrix, row, column, tempCurrentBrick, currentBrickType, tempPosRow, tempPosCol)) {
                        valid = false;
                    }
                }
                else if (OperationType_Left == operationSolution[i]) {
                    if (!Tetris::moveLeftBrick(tempBrickMatrix, row, column, tempCurrentBrick, tempPosRow, tempPosCol)) {
                        valid = false;
                    }
                }
                else if (OperationType_Right == operationSolution[i]) {
                    if (!Tetris::moveRightBrick(tempBrickMatrix, row, column, tempCurrentBrick, tempPosRow, tempPosCol)) {
                        valid = false;
                    }
                }
                else if (OperationType_ThrowDown == operationSolution[i]) {
                    Tetris::throwDownBrick(tempBrickMatrix, row, column, tempCurrentBrick, tempPosRow, tempPosCol);
                }
            }
            if (!valid) {
                continue;
            }
            // 评价操作结果
            int score = TetrisAI::evaluateBrickMatrix(tempBrickMatrix, row, column, tempCurrentBrick, tempPosRow);
            // 比上次记录的方案评价好，或者评价一样但操作步骤比上次的少，则更新最佳方案
            if (score > scoreBestSolution ||
                (score == scoreBestSolution && posOperation < operationMinCount)) {
                operationMinCount = posOperation;
                scoreBestSolution = score;
                memcpy(operationList, operationSolution, posOperation*sizeof(OperationType));
            }
        }
    }
    delete []tempBrickMatrix;
    delete []operationSolution;
    return true;
}

// 评价操作结果
int TetrisAI::evaluateBrickMatrix(BrickCellValue *brickMatrix, int row, int column,
                                  BrickCellValue (*currentBrick)[4], int posRow)
{
//    rating = (-1.0) * landingHeight          + ( 1.0) * erodedPieceCellsMetric
//    + (-1.0) * boardRowTransitions + (-1.0) * boardColTransitions
//    + (-4.0) * boardBuriedHoles 　  + (-1.0) * boardWells;
//    其中，landingHeight指当前落子落下去之后，落子中点距底部的方格数；erodedPieceCellsMetric = 消去行 * 当前落子被消去的格子数；boardRowTransitions指各行的“变换次数”之和，一行中从有方块到无方块、无方块到有方块被视为一次“变换”，游戏区域左右边界也视作有方块；boardColTransitions指各列的“变换次数”之和；boardBuriedHoles指各列中间的“空洞”方格个数之和；boardWells指各“井”的深度的连加到1的和之和（如某井深度为3，连加到1就是3+2+1），“井”指两边皆有方块的空列（“井”是在一列中，两边都有方块、而在该列中对应的位置却没有方块的这样的空段。）。
    return -1.0*TetrisAI::landingHeight(row, currentBrick, posRow) +
    1.0*TetrisAI::erodedPieceCellsMetric(brickMatrix, row, column, posRow) +
    -1.0*TetrisAI::boardRowTransitions(brickMatrix, row, column) +
    -1.0*TetrisAI::boardColTransitions(brickMatrix, row, column) +
    -4.0*TetrisAI::boardBuriedHoles(brickMatrix, row, column) +
    -1.0*TetrisAI::boardWells(brickMatrix, row, column);
}

// 求各参数的值
int TetrisAI::landingHeight(int row, BrickCellValue (*currentBrick)[4], int posRow)
{
    // 最大高度
    bool find = false;
    int maxHeight = row-posRow;
    for (int r = 0; r < 4 && !find; r++) {
        for (int c = 0; c < 4; c++) {
            if (BrickCellValue_NULL != currentBrick[r][c]) {
                find = true;
                maxHeight = row-(posRow+r);
                break;
            }
        }
    }
    // 最小高度
    find = false;
    int minHeight = row-posRow+3;
    for (int r = 3; r >= 0 && !find; r--) {
        for (int c = 0; c < 4; c++) {
            if (BrickCellValue_NULL != currentBrick[r][c]) {
                find = true;
                minHeight = row-(posRow+r);
                break;
            }
        }
    }
    return (maxHeight+minHeight)/2;
}
int TetrisAI::erodedPieceCellsMetric(BrickCellValue *brickMatrix, int row, int column, int posRow)
{
    int clearLine = 0;
    int lineIndex = 0;
    // 统计消行总量
    while (Tetris::clearOneLine(brickMatrix, row, column, posRow, lineIndex)) {
        clearLine++;
    }
    // 统计当前方块中消去的方块单元
    int cellCount = 4;
    for (int r = posRow; r < MIN(row, posRow+4); r++) {
        for (int c = 0; c < column; c++) {
            if (BrickCellValue_Reserve == brickMatrix[r*column+c]) {
                cellCount--;
            }
        }
    }
    return clearLine*cellCount;
}
int TetrisAI::boardRowTransitions(BrickCellValue *brickMatrix, int row, int column)
{
    int sum = 0;
    for (int r = 0; r < row; r++) {
        bool hasbrick = true;
        for (int c = 0; c < column; c++) {
            if (hasbrick) {
                if (BrickCellValue_NULL == brickMatrix[r*column+c]) {
                    hasbrick = false;
                    sum++;
                }
            }
            else {
                if (BrickCellValue_NULL != brickMatrix[r*column+c]) {
                    hasbrick = true;
                    sum++;
                }
            }
        }
        if (BrickCellValue_NULL == brickMatrix[r*column+column-1]) {
            sum++;
        }
    }
    return sum;
}
int TetrisAI::boardColTransitions(BrickCellValue *brickMatrix, int row, int column)
{
    int sum = 0;
    for (int c = 0; c < column; c++) {
        bool hasbrick = true;
        for (int r = 0; r < row; r++) {
            if (hasbrick) {
                if (BrickCellValue_NULL == brickMatrix[r*column+c]) {
                    hasbrick = false;
                    sum++;
                }
            }
            else {
                if (BrickCellValue_NULL != brickMatrix[r*column+c]) {
                    hasbrick = true;
                    sum++;
                }
            }
        }
        if (BrickCellValue_NULL == brickMatrix[(row-1)*column+c]) {
            sum++;
        }
    }
    return sum;
}
int TetrisAI::boardBuriedHoles(BrickCellValue *brickMatrix, int row, int column)
{
    int sum = 0;
    for (int c = 0; c < column; c++) {
        bool hasbrick = false;
        for (int r = 0; r < row; r++) {
            if (BrickCellValue_NULL == brickMatrix[r*column+c]) {
                if (hasbrick) {
                    sum++;
                }
            }
            else {
                hasbrick = true;
            }
        }
    }
    return sum;
}
int TetrisAI::boardWells(BrickCellValue *brickMatrix, int row, int column)
{
    int sum = 0;
    int wellDeep = 0;
    // 第0列的井s
    for (int r = 0; r < row; r++) {
        if (BrickCellValue_NULL == brickMatrix[r*column]) {
            if (BrickCellValue_NULL != brickMatrix[r*column+1]) {
                wellDeep++;
            }
            else {
                // 已经在井里也要计
                if (wellDeep > 0) {
                    wellDeep++;
                }
            }
        }
        else {
            // 连加到1
            while (wellDeep > 0) {
                sum += wellDeep;
                wellDeep--;
            }
        }
    }
    // 连加到1
    while (wellDeep > 0) {
        sum += wellDeep;
        wellDeep--;
    }
    // 最后一列的井
    for (int r = 0; r < row; r++) {
        if (BrickCellValue_NULL == brickMatrix[r*column+column-1]) {
            if (BrickCellValue_NULL != brickMatrix[r*column+column-2]) {
                wellDeep++;
            }
            else {
                // 已经在井里也要计
                if (wellDeep > 0) {
                    wellDeep++;
                }
            }
        }
        else {
            // 连加到1
            while (wellDeep > 0) {
                sum += wellDeep;
                wellDeep--;
            }
        }
    }
    // 连加到1
    while (wellDeep > 0) {
        sum += wellDeep;
        wellDeep--;
    }
    // 中间列的井
    for (int c = 1; c < column-1; c++) {
        for (int r = 0; r < row; r++) {
            if (BrickCellValue_NULL == brickMatrix[r*column+c]) {
                // 两边均有方块
                if (BrickCellValue_NULL != brickMatrix[r*column+c-1] &&
                    BrickCellValue_NULL != brickMatrix[r*column+c+1]) {
                    wellDeep++;
                }
                // 两边不全有方块
                else {
                    // 但已经在井里也要计
                    if (wellDeep > 0) {
                        wellDeep++;
                    }
                }
            }
            else {
                // 连加到1
                while (wellDeep > 0) {
                    sum += wellDeep;
                    wellDeep--;
                }
            }
        }
        // 连加到1
        while (wellDeep > 0) {
            sum += wellDeep;
            wellDeep--;
        }
    }
    return sum;
}