#ifndef __ProtectPrincessGame__GameScene__
#define __ProtectPrincessGame__GameScene__

#include "cocos2d.h"
#include "utils\PointDelegate.h"
#include "utils\MonsterData.h"
#include "SimpleAudioEngine.h"
#include "utils\TurretData.h"
#include "json/document.h"
#include "LevelManager.h"
#include "GameMenuScene.h"
#include "Sprites\SpriteBase.h"
#include "Sprites\Turret.h"
#include "Sprites\Bullet.h"
#include "Sprites\Monster.h"

USING_NS_CC;
using namespace CocosDenshion;

// 游戏逻辑类
class Game : public cocos2d::Layer
{
protected:
    TMXTiledMap* _tileMap;  // 地图
    TMXLayer* _collidable;  // 障碍层
    std::string _tileFile; // 关卡地图名称
    int _number; // 关卡共出现多少波怪物
    int _currentCount; // 当前出现怪物数量
    int _currNum; // 当前出现怪物波数
    int _goldValue; // 玩家当前的金币值
    Sprite* _princess; // 公主
    int _screenWidth , _screenHeight;  // 屏幕宽度和高度
    int _count;  // 游戏帧计数器
    int _delivery; // 出现怪物取模基数
    
    Vector<MonsterData*> _monsterDatas; // 当前关卡的怪物信息
    Vector<TurretData*> _turretDatas; // 当前关卡的炮塔信息
    Vector<PointDelegate*> _pathPoints; // 记录有效路径的点

	Label* _numberLabel; // 显示怪物波数的Label
    Label* _currNumLabel; // 显示当前怪物波数的Label
    Label* _goldLabel; // 显示当前玩家金币的Label

	Vector<Monster*> _monsterVector;  // 存储出现怪物的集合
    bool _isFinish ; // 关卡所有怪物是否全部出现标记
    bool _isSelect ; // 判断用户触碰是选择位置还是建造炮塔

    Vector<Turret*> _turretVector; // 炮塔集合，存放已经添加的炮塔
    Vector<Bullet*> _bulletVector; // 炮弹集合

	public:
    /**
     * 静态函数，创建Scene场景
     * 注意：在Cocos2d-x中没有Cocos2d-iPhone中的“id”,建议返回类实例的指针返回类实例的指针
     * selectLevel：当前关卡数
     */
    static cocos2d::Scene* createSceneWithLevel(int selectLevel);
    
    /**
     * init函数，完成初始化操作。
     * 注意：init函数在Cocos2d-x中返回bool值，而不是返回Cocos2d-iPhone中的“id”
     */
    virtual bool init();
    
    // TileMap坐标转换为OpenGL坐标
    Point locationForTilePos(Point pos);
    // 将OpenGL坐标转换为TileMap坐标
    Point tileCoordForPosition(Point position);

	void update(float delta);

	 /**
     * 获得动画动作
     * animName：动画帧的名字
     * delay：动画帧与帧之间的间隔时间
     * animNum：动画帧的数量
     */
    Animate* getAnimateByName(std::string animName,float delay,int animNum);
    /**
     * 怪物根据路径点移动
     * pathVector：从关卡json文件中获取的路径集合
     * monster：移动的怪物
     */
    void monsterMoveWithWayPoints(Vector<PointDelegate*> pathVector, Monster* monster);
    // 更新HUD
    void updateHUD(float delta);
    // 随机出现怪物
    void updateMonster(float delta);
    
    
    /**
     * 检测攻击范围，表示进入攻击范围，false，表示没有进入攻击范围
     * monsterPoint 怪物所在的坐标点
     * turretPoint 炮塔所在的坐标点
     * area 攻击范围
     */
    bool checkPointInCircle(Point monsterPoint, Point turretPoint, int area);
    /**
     * 发射炮弹时计算炮弹的旋转方向，返回表示旋转方向的浮点数
     * monsterPoint 怪物所在的坐标点
     * turret 炮塔
     */
    float getTurretRotation(Point monsterPoint,Turret* turret);
    /**
     * 根据炮弹移动距离计算炮弹需要移动时间，返回表示时间的浮点数
     * start 炮弹移动的开始坐标
     * end 炮弹移动的结束坐标
     */
    float getBulletMoveTime(Point start, Point end);
    // 检测坐标点是否障碍
    bool getCollidable(Point position);
    // 检测炮塔
    void detectionTurret(float delta);
    
    // 检测碰撞
    void collisionDetection(float delta);
    // 检测公主
    void detectionPrincess(float delta);
    
    /**
     游戏结束，tag，0表示游戏失败，1表示游戏胜利
     */
    void gameOver(int tag);
    CREATE_FUNC(Game);
};

#endif //__ProtectPrincessGame__GameScene__
    