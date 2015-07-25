#include "Tetris.h"
//102 228 226 232 108 198 240
unsigned char Tetris::m_BrickMode[7] = {0x66,0xe4,0xe2,0xe8,0x6c,0xc6,0xf0};

Tetris::Tetris(int row,int column)
{
	if(row < MinRow){
		row = MinRow;
	}
	if( column < MinColumn){
		column = MinColumn;
	}
	m_row = row;
	m_column = column;
	long size = m_row*m_column;
	m_brickMatrix = new BrickCellValue[size];
	//��ʼ��������Ϸ����ÿ�����鵥Ԫֵ
	for( int i = 0; i < size; i++){
		m_brickMatrix[i] = BrickCellValue_NULL;
	}
}

Tetris::~Tetris()
{
	delete[] m_brickMatrix;
}

//ָ���кź��кŵķ��鵥Ԫֵ
BrickCellValue Tetris::getBrickValue(int r, int c)
{
	if((r >= 0 && r < m_row) && (c >= 0 && c < m_column)){
		// �������ĸ�λ���з����򷵻ظ�ֵ
		if(BrickCellValue_NULL != m_brickMatrix[r*m_column+c]){
			return m_brickMatrix[r*m_column+c];
		}
		// �������(r,c)Ϊ��ǰ�����ϵ�ֵ���򷵻ص�ǰ������Ӧλ�õ�ֵ
		else if((r >= m_posRow && r < m_posRow+4) && (c >= m_posCol && c < m_posCol+4)){
			return m_currentBrick[r-m_posRow][c-m_posCol];
		}
	}
	return BrickCellValue_NULL;
}

//��շ������
void Tetris::clearBrickMatrix()
{
	for(int i = 0; i < m_row*m_column;i++){
		m_brickMatrix[i] = BrickCellValue_NULL;
	}
}

bool Tetris::setNewBrick(BrickType brickType,BrickCellValue brickCellValue)
{
	m_currentBrickType = brickType;
	return Tetris::setNewBrick(m_brickMatrix,m_row,m_column,
		m_currentBrick,brickType,brickCellValue,m_posRow,m_posCol);
}

bool Tetris::moveLeftBrick()
{
	return Tetris::moveLeftBrick(m_brickMatrix,m_row,m_column,m_currentBrick,m_posRow,m_posCol);
}
bool Tetris::moveRightBrick()
{
    return Tetris::moveRightBrick(m_brickMatrix, m_row, m_column, m_currentBrick, m_posRow, m_posCol);
}
bool Tetris::rotateBrick()
{
    return Tetris::rotateBrick(m_brickMatrix, m_row, m_column, m_currentBrick, m_currentBrickType, m_posRow, m_posCol);
}
bool Tetris::fallBrick()
{
    return Tetris::fallBrick(m_brickMatrix, m_row, m_column, m_currentBrick, m_posRow, m_posCol);
}
void Tetris::throwDownBrick()
{
    Tetris::throwDownBrick(m_brickMatrix, m_row, m_column, m_currentBrick, m_posRow, m_posCol);
}
bool Tetris::clearOneLine(int &lineIndex)
{
    return Tetris::clearOneLine(m_brickMatrix, m_row, m_column, m_posRow, lineIndex);
}

bool Tetris::setNewBrick(BrickCellValue *brickMatrix,int row,int column,
						 BrickCellValue currentBrick[4][4],BrickType brickType,BrickCellValue brickCellValue,
						 int &posRow, int &posCol)
{
	if(brickType <= BrickType_NULL || brickType >= BrickType_Count){
		return false;
	}
	// ��ǰ����λ������
	posRow = -1;
	posCol = (column-4)/2;
	//���ԭ�з���
	for(int r = 0;r < 4;r++){
		for(int c = 0; c < 4; c++){
			currentBrick[r][c] = BrickCellValue_NULL;
		}
	}
	// ����ģ����䷽��
	for(int r = 0; r < 2; r++){
		for(int c = 0; c < 4; c++){
			if(((Tetris::m_BrickMode[brickType]<<(r*4+c)) & 0x80) > 0){
				currentBrick[1+r][c] = brickCellValue;
				// �����������λ����Ч���ҷ����������λ��Ҳ��ֵ������Ϸ����
				//���鿪ʼ���ֵ�λ��Ϊ���м�4567 16171819
				int offset = (posRow + (1+r)) * column+(posCol+c);
				if(posRow+(1+r) >= 0 && BrickCellValue_NULL != brickMatrix[offset]){
					return false;
				}
			}
		}
	}
	return true;
}

bool Tetris::rotateBrick(BrickCellValue *brickMatrix,int row,int column,
						 BrickCellValue currentBrick[4][4],BrickType currentBrickType,int posRow,int posCol)
{
	// �����β���ת
	if(BrickType_o == currentBrickType){
		return false;
	}
	// ����ʱ����������תһ��
	BrickCellValue tempBrick[4][4];
	for(int r = 0; r < 4 ; r++){
		for(int c = 0 ; c < 4; c++){
			tempBrick[r][c] = BrickCellValue_NULL;
		}
	}
	// ���η�����ת
	if(BrickType_1 == currentBrickType){
		//����
		if(BrickCellValue_NULL != currentBrick[0][1]){
			for(int i = 0; i < 4; i++){
				tempBrick[1][i] = currentBrick[i][1];
			}
		}
		// ����
		else{
			for(int i = 0; i < 4; i++){
				tempBrick[i][1] = currentBrick[1][i];
			}
		}
	}
	// s�η�����ת
	else if(BrickType_s == currentBrickType){
		//����
		if(BrickCellValue_NULL != currentBrick[0][0]){
            tempBrick[1][1] = currentBrick[1][1];
            tempBrick[1][2] = currentBrick[1][1];
            tempBrick[2][0] = currentBrick[1][1];
            tempBrick[2][1] = currentBrick[1][1];
		}
		// ����
        else {
            tempBrick[0][0] = currentBrick[1][1];
            tempBrick[1][0] = currentBrick[1][1];
            tempBrick[1][1] = currentBrick[1][1];
            tempBrick[2][1] = currentBrick[1][1];
        }
	}
	// z�η�����ת
    else if (BrickType_z == currentBrickType) {
        // ����
        if (BrickCellValue_NULL != currentBrick[0][1]) {
            tempBrick[1][0] = currentBrick[1][1];
            tempBrick[1][1] = currentBrick[1][1];
            tempBrick[2][1] = currentBrick[1][1];
            tempBrick[2][2] = currentBrick[1][1];
        }
        // ����
        else {
            tempBrick[0][1] = currentBrick[1][1];
            tempBrick[1][0] = currentBrick[1][1];
            tempBrick[1][1] = currentBrick[1][1];
            tempBrick[2][0] = currentBrick[1][1];
        }
    }
    // ����������ת
    else {
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
                tempBrick[r][c] = currentBrick[2-c][r];
            }
        }
    }
	//�鿴��ת���뷽������Ƿ��ͻ
	for(int r = 0; r < 4; r++){
		for(int c = 0; c < 4; c++){
			// ��ǰ��������λ����ֵ
			if(BrickCellValue_NULL != tempBrick[r][c]){
				// ��ߣ������ұߣ������±߳����߽磬��תʧ��
				if(posCol+c < 0 || posCol+c >= column || posRow+r >= row){
					return false;
				}
				// �����������λ����Ч���ҷ����������λ��Ҳ��ֵ������תʧ��
                int offset = (posRow+r)*column+(posCol+c);
                if (offset >= 0 && offset < row*column &&
                    BrickCellValue_NULL != brickMatrix[offset]) {
                    return false;
                }
			}
		}
	}
	// ����ͻ����ת
	for(int r = 0; r < 4; r++){
		for(int c = 0; c < 4; c++){
			currentBrick[r][c] = tempBrick[r][c];
		}
	}
	return true;
}

bool Tetris::moveLeftBrick(BrickCellValue *brickMatrix, int row, int column,
                           BrickCellValue currentBrick[4][4], int posRow, int &posCol)
{
	// ��������һ��
	int tempPosCol = posCol - 1;
	for(int r = 0; r < 4; r++){
		for (int c = 0; c < 4; c++) {
			if (BrickCellValue_NULL != currentBrick[r][c]) {
                // ������߽磬����ʧ��
                if (tempPosCol+c < 0) {
                    return false;
                }
                // �����������λ����Ч���ҷ����������λ��Ҳ��ֵ��������ʧ��
                int offset = (posRow+r)*column+(tempPosCol+c);
                if (posRow+r >= 0 && posRow+r < row &&
                    BrickCellValue_NULL != brickMatrix[offset]) {
                    return false;
                }
            }
		}
	}
	// ��������
    posCol -= 1;
    return true;
}

bool Tetris::moveRightBrick(BrickCellValue *brickMatrix, int row, int column,
                            BrickCellValue currentBrick[4][4], int posRow, int &posCol)
{
    // ��������һ��
    int tempPosCol = posCol+1;
    for (int r = 0; r < 4; r++) {
        for (int c = 3; c >= 0; c--) { // ���������жϿ��ܻ��Щ
            // ��ǰ��������λ����ֵ
            if (BrickCellValue_NULL != currentBrick[r][c]) {
                // �����ұ߽磬����ʧ��
                if (tempPosCol+c >= column) {
                    return false;
                }
                // �����������λ����Ч���ҷ����������λ��Ҳ��ֵ��������ʧ��
                int offset = (posRow+r)*column+(tempPosCol+c);
                if (posRow+r >= 0 && posRow+r < row &&
                    BrickCellValue_NULL != brickMatrix[offset]) {
                    return false;
                }
            }
        }
    }
    // ��������
    posCol += 1;
    return true;
}

bool Tetris::fallBrick(BrickCellValue *brickMatrix, int row, int column,
                       BrickCellValue currentBrick[4][4], int &posRow, int posCol)
{
    // ��������һ��
    int tempPosRow = posRow+1;
	// ���������жϿ��ܻ��Щ
    for (int r = 3; r >= 0; r--) { 
        for (int c = 0; c < 4; c++) {
            // ��ǰ��������λ����ֵ
            if (BrickCellValue_NULL != currentBrick[r][c]) {
                // �����±߽磬����ʧ��
                if (tempPosRow+r >= row) {
                    // ����ǰ����̶������������
                    Tetris::fixBrick(brickMatrix, row, column, currentBrick, posRow, posCol);
                    return false;
                }
                // δ�����±߽磬�����λ���ڷ�������ڣ��ҷ����������λ��Ҳ��ֵ��������ʧ��
                else if (tempPosRow+r >= 0 && BrickCellValue_NULL != brickMatrix[(tempPosRow+r)*column+(posCol+c)]) {
                    // ����ǰ����̶������������
                    Tetris::fixBrick(brickMatrix, row, column, currentBrick, posRow, posCol);
                    return false;
                }
            }
        }
    }
    // ��������
    posRow += 1;
    return true;
}

void Tetris::throwDownBrick(BrickCellValue *brickMatrix, int row, int column,
                            BrickCellValue currentBrick[4][4], int &posRow, int posCol)
{
    while (Tetris::fallBrick(brickMatrix, row, column, currentBrick, posRow, posCol));
}

void Tetris::fixBrick(BrickCellValue *brickMatrix, int row, int column,
                      BrickCellValue currentBrick[4][4], int posRow, int posCol)
{
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (BrickCellValue_NULL != currentBrick[r][c]) {
				//���鲻�ٶ�ʱ��������ֵ������Ϸ�ľ�����
                brickMatrix[(posRow+r)*column+(posCol+c)] = currentBrick[r][c];
            }
        }
    }
}

bool Tetris::clearOneLine(BrickCellValue *brickMatrix, int row, int column,
                          int posRowCurrentBrick, int &lineIndex)
{
	int rStart = MIN(row-1,posRowCurrentBrick+4);
	int rEnd = MAX(0,posRowCurrentBrick);
	for(int r = rStart; r >= rEnd; r--){
		// �Ȳ鿴�����Ƿ��������
		bool canClearLine = true;
		for(int c = 0; c < column; c++){
			if(BrickCellValue_NULL == brickMatrix[r*column+c]){
				canClearLine = false;
				break;
			}
		}
		// ��������У����������һ��
		if(canClearLine){
			lineIndex = r;
			// �������¸�ֵ���ڴ���λ������
			memmove(brickMatrix+column, brickMatrix, r*column*sizeof(BrickCellValue));
			// ��0�����
            for (int c = 0; c < column; c++) {
                brickMatrix[c] = BrickCellValue_NULL;
            }
			return true;
		}
	}
	return false;
}
