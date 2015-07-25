#include "GameLayer.h"
#include "SimpleAudioEngine.h"

bool GameLayer::init()
{
	if (!Layer::init()) {
        return false;
    }
	m_gameRun = false;
    m_actionAI = NULL;
    m_speedAI = 0.2f;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	// ��Ϸ��
	float leftBrickMatrix = 120;
	//���鷽�����
	m_spriteBrick = new Sprite*[Row_Tetris*Column_Tetris];
	for (int r = 0; r < Row_Tetris; r++) {
        for (int c = 0; c < Column_Tetris; c++) {
            m_spriteBrick[r*Column_Tetris+c] = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("tetris_0.png"));
            m_spriteBrick[r*Column_Tetris+c]->setPosition(leftBrickMatrix+c*16+8,
                                                          visibleSize.height/2+153-r*16+8);
            this->addChild(m_spriteBrick[r*Column_Tetris+c]);
        }
    }
    m_tetris = new Tetris(Row_Tetris, Column_Tetris);
    return true;
}

void GameLayer::startGame()
{
	m_gameRun = true;
    // ��շ������
    m_tetris->clearBrickMatrix();
    // �����·���
	BrickType brickType = (BrickType)(rand()%BrickType_Count);
	m_tetris->setNewBrick(brickType, (BrickCellValue)(brickType+1));
    // ˢ�·������
    this->updateTetrisBrickMatrix();
	this->startAI(false);
	this->stopAllActions();
    CallFunc *callFunc = CallFunc::create([=]{
        if (m_gameRun) {
            this->fallBrick();
        }
        else {
            Size visibleSize = Director::getInstance()->getVisibleSize();
            Label *labelGameOver = Label::createWithSystemFont("GameOver", "", 40);
            labelGameOver->setColor(Color3B::RED);
            labelGameOver->setPosition(visibleSize.width/2, visibleSize.height/2);
            this->addChild(labelGameOver);
            labelGameOver->runAction(Sequence::create(DelayTime::create(2), CallFuncN::create([=](Node *node){node->removeFromParent();this->stopAllActions();}), NULL));
        }
    });
    this->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1), callFunc, NULL)));
}

void GameLayer::startAI(bool AIOn)
{
    if (!m_gameRun) {
        return;
    }
    if (AIOn) {
        // ���
        if (TetrisAI::seekSolution(m_tetris->getBrickMatrix(), m_tetris->getRow(), m_tetris->getColumn(), m_tetris->getCurrentBrick(), m_tetris->getCurrentBrickType(), m_tetris->getPosRowOfCurrentBrick(), m_tetris->getPosColOfCurrentBrick(), m_operationList)) {
            this->commitOperationList(0);
        }
    }
    else {
        if (NULL != m_actionAI) {
            this->stopAction(m_actionAI);
            m_actionAI = NULL;
        }
    }
}
void GameLayer::accelerateAI()
{
    m_speedAI -= 0.05f;
    if (m_speedAI < 0.05f) {
        m_speedAI = 0.05f;
    }
}
void GameLayer::decelerateAI()
{
    m_speedAI += 0.05f;
    if (m_speedAI > 1) {
        m_speedAI = 1;
    }
}
// �������
void GameLayer::moveLeftBrick()
{
    if (m_tetris->moveLeftBrick()) {
        // ˢ�·������
        this->updateTetrisBrickMatrix();
    }
}
void GameLayer::moveRightBrick()
{
    if (m_tetris->moveRightBrick()) {
        // ˢ�·������
        this->updateTetrisBrickMatrix();
    }
}
void GameLayer::rotateBrick()
{
    if (m_tetris->rotateBrick()) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/rotate.wav");
        // ˢ�·������
        this->updateTetrisBrickMatrix();
    }
}
void GameLayer::fallBrick()
{
    if (!m_tetris->fallBrick())
    {
        int lineIndex = 0;
        while (m_tetris->clearOneLine(lineIndex)) {
            printf("���У�%d\n", lineIndex);
        }
        // ��һ������
        BrickType brickType = (BrickType)(rand()%BrickType_Count);
        if (!m_tetris->setNewBrick(brickType, (BrickCellValue)(brickType+1)))
        {
            m_gameRun = false;
            printf("����---\n");
        }
    }
    // ˢ�·������
    this->updateTetrisBrickMatrix();
}
void GameLayer::throwDownBrick()
{
    m_tetris->throwDownBrick();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/throwdown.wav");
    int lineIndex = 0;
    while (m_tetris->clearOneLine(lineIndex)) {
        printf("���У�%d\n", lineIndex);
    }
    // ��һ������
    BrickType brickType = (BrickType)(rand()%BrickType_Count);
    if (!m_tetris->setNewBrick(brickType, (BrickCellValue)(brickType+1)))
    {
        m_gameRun = false;
        printf("����---\n");
    }
    // ˢ�·������
    this->updateTetrisBrickMatrix();
}

// ˢ�·������
void GameLayer::updateTetrisBrickMatrix()
{
    for (int r = 0; r < Row_Tetris; r++) {
        for (int c = 0; c < Column_Tetris; c++) {
            std::string filePath = StringUtils::format("tetris_%i.png", m_tetris->getBrickValue(r, c));
            m_spriteBrick[r*Column_Tetris+c]->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey(filePath));
        }
    }
    // ��log����ʾ��Ϸ
    this->showTetrisInLog();
}
// ��log����ʾ��Ϸ
void GameLayer::showTetrisInLog()
{
    for (int r = 0; r < m_tetris->getColumn()+2; r++) {
        printf("_");
    }
    printf("\n");
    for (int r = 0; r < m_tetris->getRow(); r++) {
        printf("|");
        for (int c = 0; c < m_tetris->getColumn(); c++) {
            printf("%s", m_tetris->getBrickValue(r, c)>0?"1":" ");
        }
        printf("|\n");
    }
    for (int r = 0; r < m_tetris->getColumn()+2; r++) {
        printf("T");
    }
    printf("\n");
}
// ִ��AIϵͳ���صĲ����б�
void GameLayer::commitOperationList(int index)
{
    if (OperationType_ThrowDown != m_operationList[index]) {
        switch (m_operationList[index]) {
            case OperationType_Rotate:
            {
                this->rotateBrick();
            }
                break;
            case OperationType_Left:
            {
                this->moveLeftBrick();
            }
                break;
            case OperationType_Right:
            {
                this->moveRightBrick();
            }
                break;
            default:
                break;
        }
        if (m_gameRun) {
            m_actionAI = Sequence::create(DelayTime::create(m_speedAI), CallFunc::create([=]{this->commitOperationList(index+1);}), NULL);
            this->runAction(m_actionAI);
        }
    }
    else {
        this->throwDownBrick();
        if (m_gameRun) {
            TetrisAI::seekSolution(m_tetris->getBrickMatrix(), m_tetris->getRow(), m_tetris->getColumn(), m_tetris->getCurrentBrick(), m_tetris->getCurrentBrickType(), m_tetris->getPosRowOfCurrentBrick(), m_tetris->getPosColOfCurrentBrick(), m_operationList);
            m_actionAI = Sequence::create(DelayTime::create(m_speedAI), CallFunc::create([=]{this->commitOperationList(0);}), NULL);
            this->runAction(m_actionAI);
        }
    }
}

