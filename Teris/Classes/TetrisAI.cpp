#include "TetrisAI.h"

// currentBrickΪ����ָ�룬���鳤�ȹ̶�Ϊ4�����������ȱʡ��
// ���������Ĳ��������Ϊ(3+column/2+1)
bool TetrisAI::seekSolution(BrickCellValue *brickMatrix, int row, int column,
                            BrickCellValue (*currentBrick)[4], BrickType currentBrickType,
                            int posRow, int posCol, OperationType operationList[])
{
    operationList[0] = OperationType_ThrowDown;
    // ����ռ�����ִ�в�������
    int tempPosRow = posRow, tempPosCol = posCol;
    BrickCellValue *tempBrickMatrix = new BrickCellValue[row*column];
    BrickCellValue tempCurrentBrick[4][4];
    // ���Ʒ�����󼰵�ǰ����
    memcpy(tempBrickMatrix, brickMatrix, row*column*sizeof(BrickCellValue));
    memcpy(tempCurrentBrick, currentBrick, 4*4*sizeof(BrickCellValue));
    // ���õ�ǰ����ʧ�����ʾ��Ϸ������ֱ�ӷ��ؼ���
    if (!Tetris::setNewBrick(tempBrickMatrix, row, column, tempCurrentBrick, currentBrickType, BrickCellValue_1, tempPosRow, tempPosCol)) {
        delete []tempBrickMatrix;
        return false;
    }
    // ��ǰ�����з���ĵ�Ԫֵ��ΪBrickCellValue_Reserve���Ա�ͳ����ȥ������
    BrickCellValue reserveCurrentBrick[4][4];
    memcpy(reserveCurrentBrick, currentBrick, 4*4*sizeof(BrickCellValue));
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c <4; c++) {
            if (BrickCellValue_NULL != reserveCurrentBrick[r][c]) {
                reserveCurrentBrick[r][c] = BrickCellValue_Reserve;
            }
        }
    }
    
    // ÿ�ַ������ת�Ĵ���
    int rotateCount = 4;
    if (BrickType_o == currentBrickType) {
        rotateCount = 1;
    }
    else if (BrickType_1 == currentBrickType ||
             BrickType_s == currentBrickType ||
             BrickType_z == currentBrickType) {
        rotateCount = 2;
    }
    // ֻ����һ�ַ����Ŀռ�����ִ�м���
    int operationCount = 3+column/2+1;
    OperationType *operationSolution = new OperationType[operationCount];
    // ֻ��ס��ǰѡ�еķ����ĵ÷ּ���
    int scoreBestSolution = -10000;
    // �����һ�²���������������Ա������۷���ͬ�������ѡ�����������ٵ�
    int operationMinCount = operationCount;
    // ������ת��������
    for (int rotate = 0; rotate < rotateCount; rotate++) {
        // ����ƽ�Ƶ�������
        for (int move = 0; move < column; move++) {
            // ��ԭ������󡢵�ǰ���鼰λ��
            memcpy(tempBrickMatrix, brickMatrix, row*column*sizeof(BrickCellValue));
            memcpy(tempCurrentBrick, reserveCurrentBrick, 4*4*sizeof(BrickCellValue));
            tempPosRow = posRow, tempPosCol = posCol;
            // �÷�����λ��
            int posOperation = 0;
            // ��ת����
            for (int r = 0; r < rotate; r++) {
                operationSolution[posOperation++] = OperationType_Rotate;
            }
            // ���ƴ���
            if (move < column/2-1) {
                for (int m = 0; m < (column/2-1)-move; m++) { // �������Ϊ��5
                    operationSolution[posOperation++] = OperationType_Left;
                }
            }
            // ���ƴ���
            else if (move > column/2-1) {
                for (int m = 0; m < move-(column/2-1); m++) { // �������Ϊ��6
                    operationSolution[posOperation++] = OperationType_Right;
                }
            }
            // ���ƶ�
            else {
            }
            // ׷�Ӷ��²���
            operationSolution[posOperation++] = OperationType_ThrowDown;
            // ���Ը÷����Ƿ����
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
            // ���۲������
            int score = TetrisAI::evaluateBrickMatrix(tempBrickMatrix, row, column, tempCurrentBrick, tempPosRow);
            // ���ϴμ�¼�ķ������ۺã���������һ��������������ϴε��٣��������ѷ���
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

// ���۲������
int TetrisAI::evaluateBrickMatrix(BrickCellValue *brickMatrix, int row, int column,
                                  BrickCellValue (*currentBrick)[4], int posRow)
{
//    rating = (-1.0) * landingHeight          + ( 1.0) * erodedPieceCellsMetric
//    + (-1.0) * boardRowTransitions + (-1.0) * boardColTransitions
//    + (-4.0) * boardBuriedHoles ��  + (-1.0) * boardWells;
//    ���У�landingHeightָ��ǰ��������ȥ֮�������е��ײ��ķ�������erodedPieceCellsMetric = ��ȥ�� * ��ǰ���ӱ���ȥ�ĸ�������boardRowTransitionsָ���еġ��任������֮�ͣ�һ���д��з��鵽�޷��顢�޷��鵽�з��鱻��Ϊһ�Ρ��任������Ϸ�������ұ߽�Ҳ�����з��飻boardColTransitionsָ���еġ��任������֮�ͣ�boardBuriedHolesָ�����м�ġ��ն����������֮�ͣ�boardWellsָ������������ȵ����ӵ�1�ĺ�֮�ͣ���ĳ�����Ϊ3�����ӵ�1����3+2+1����������ָ���߽��з���Ŀ��У�����������һ���У����߶��з��顢���ڸ����ж�Ӧ��λ��ȴû�з���������ĿնΡ�����
    return -1.0*TetrisAI::landingHeight(row, currentBrick, posRow) +
    1.0*TetrisAI::erodedPieceCellsMetric(brickMatrix, row, column, posRow) +
    -1.0*TetrisAI::boardRowTransitions(brickMatrix, row, column) +
    -1.0*TetrisAI::boardColTransitions(brickMatrix, row, column) +
    -4.0*TetrisAI::boardBuriedHoles(brickMatrix, row, column) +
    -1.0*TetrisAI::boardWells(brickMatrix, row, column);
}

// ���������ֵ
int TetrisAI::landingHeight(int row, BrickCellValue (*currentBrick)[4], int posRow)
{
    // ���߶�
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
    // ��С�߶�
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
    // ͳ����������
    while (Tetris::clearOneLine(brickMatrix, row, column, posRow, lineIndex)) {
        clearLine++;
    }
    // ͳ�Ƶ�ǰ��������ȥ�ķ��鵥Ԫ
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
    // ��0�еľ�s
    for (int r = 0; r < row; r++) {
        if (BrickCellValue_NULL == brickMatrix[r*column]) {
            if (BrickCellValue_NULL != brickMatrix[r*column+1]) {
                wellDeep++;
            }
            else {
                // �Ѿ��ھ���ҲҪ��
                if (wellDeep > 0) {
                    wellDeep++;
                }
            }
        }
        else {
            // ���ӵ�1
            while (wellDeep > 0) {
                sum += wellDeep;
                wellDeep--;
            }
        }
    }
    // ���ӵ�1
    while (wellDeep > 0) {
        sum += wellDeep;
        wellDeep--;
    }
    // ���һ�еľ�
    for (int r = 0; r < row; r++) {
        if (BrickCellValue_NULL == brickMatrix[r*column+column-1]) {
            if (BrickCellValue_NULL != brickMatrix[r*column+column-2]) {
                wellDeep++;
            }
            else {
                // �Ѿ��ھ���ҲҪ��
                if (wellDeep > 0) {
                    wellDeep++;
                }
            }
        }
        else {
            // ���ӵ�1
            while (wellDeep > 0) {
                sum += wellDeep;
                wellDeep--;
            }
        }
    }
    // ���ӵ�1
    while (wellDeep > 0) {
        sum += wellDeep;
        wellDeep--;
    }
    // �м��еľ�
    for (int c = 1; c < column-1; c++) {
        for (int r = 0; r < row; r++) {
            if (BrickCellValue_NULL == brickMatrix[r*column+c]) {
                // ���߾��з���
                if (BrickCellValue_NULL != brickMatrix[r*column+c-1] &&
                    BrickCellValue_NULL != brickMatrix[r*column+c+1]) {
                    wellDeep++;
                }
                // ���߲�ȫ�з���
                else {
                    // ���Ѿ��ھ���ҲҪ��
                    if (wellDeep > 0) {
                        wellDeep++;
                    }
                }
            }
            else {
                // ���ӵ�1
                while (wellDeep > 0) {
                    sum += wellDeep;
                    wellDeep--;
                }
            }
        }
        // ���ӵ�1
        while (wellDeep > 0) {
            sum += wellDeep;
            wellDeep--;
        }
    }
    return sum;
}