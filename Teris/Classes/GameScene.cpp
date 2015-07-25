#include "GameScene.h"

bool GameScene::init()
{
    if (!Scene::init()) {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // ±³¾°
    Sprite *spriteBG = Sprite::create("tetris_bg.png");
    spriteBG->setPosition(visibleSize.width/2, visibleSize.height/2);
    this->addChild(spriteBG);
    
    // ÓÎÏ·¿ØÖÆ²ã
    GameCtrlLayer *gameCtrlLayer = GameCtrlLayer::create();
    gameCtrlLayer->setDelegate(this);
    this->addChild(gameCtrlLayer);
    // ÓÎÏ·²ã
    m_gameLayer = GameLayer::create();
    this->addChild(m_gameLayer);
    return true;
}

// GameCtrlLayerDelegate
void GameScene::gameCtrlLayerClickStartGame()
{
    m_gameLayer->startGame();
}
void GameScene::gameCtrlLayerStartAI(bool AIOn)
{
    m_gameLayer->startAI(AIOn);
}
void GameScene::gameCtrlLayerClickAccelerate()
{
    m_gameLayer->accelerateAI();
}
void GameScene::gameCtrlLayerClickDecelerate()
{
    m_gameLayer->decelerateAI();
}
float GameScene::gameCtrlLayerGetAISpeed()
{
    return m_gameLayer->getAISpeed();
}
void GameScene::gameCtrlLayerClickLeft()
{
    m_gameLayer->moveLeftBrick();
}
void GameScene::gameCtrlLayerClickRight()
{
    m_gameLayer->moveRightBrick();
}
void GameScene::gameCtrlLayerClickRotate()
{
    m_gameLayer->rotateBrick();
}
void GameScene::gameCtrlLayerClickThrowDown()
{
    m_gameLayer->throwDownBrick();
}