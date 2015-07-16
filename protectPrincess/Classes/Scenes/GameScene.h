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

// ��Ϸ�߼���
class Game : public cocos2d::Layer
{
protected:
    TMXTiledMap* _tileMap;  // ��ͼ
    TMXLayer* _collidable;  // �ϰ���
    std::string _tileFile; // �ؿ���ͼ����
    int _number; // �ؿ������ֶ��ٲ�����
    int _currentCount; // ��ǰ���ֹ�������
    int _currNum; // ��ǰ���ֹ��ﲨ��
    int _goldValue; // ��ҵ�ǰ�Ľ��ֵ
    Sprite* _princess; // ����
    int _screenWidth , _screenHeight;  // ��Ļ��Ⱥ͸߶�
    int _count;  // ��Ϸ֡������
    int _delivery; // ���ֹ���ȡģ����
    
    Vector<MonsterData*> _monsterDatas; // ��ǰ�ؿ��Ĺ�����Ϣ
    Vector<TurretData*> _turretDatas; // ��ǰ�ؿ���������Ϣ
    Vector<PointDelegate*> _pathPoints; // ��¼��Ч·���ĵ�

	Label* _numberLabel; // ��ʾ���ﲨ����Label
    Label* _currNumLabel; // ��ʾ��ǰ���ﲨ����Label
    Label* _goldLabel; // ��ʾ��ǰ��ҽ�ҵ�Label

	Vector<Monster*> _monsterVector;  // �洢���ֹ���ļ���
    bool _isFinish ; // �ؿ����й����Ƿ�ȫ�����ֱ��
    bool _isSelect ; // �ж��û�������ѡ��λ�û��ǽ�������

    Vector<Turret*> _turretVector; // �������ϣ�����Ѿ���ӵ�����
    Vector<Bullet*> _bulletVector; // �ڵ�����

	public:
    /**
     * ��̬����������Scene����
     * ע�⣺��Cocos2d-x��û��Cocos2d-iPhone�еġ�id��,���鷵����ʵ����ָ�뷵����ʵ����ָ��
     * selectLevel����ǰ�ؿ���
     */
    static cocos2d::Scene* createSceneWithLevel(int selectLevel);
    
    /**
     * init��������ɳ�ʼ��������
     * ע�⣺init������Cocos2d-x�з���boolֵ�������Ƿ���Cocos2d-iPhone�еġ�id��
     */
    virtual bool init();
    
    // TileMap����ת��ΪOpenGL����
    Point locationForTilePos(Point pos);
    // ��OpenGL����ת��ΪTileMap����
    Point tileCoordForPosition(Point position);

	void update(float delta);

	 /**
     * ��ö�������
     * animName������֡������
     * delay������֡��֮֡��ļ��ʱ��
     * animNum������֡������
     */
    Animate* getAnimateByName(std::string animName,float delay,int animNum);
    /**
     * �������·�����ƶ�
     * pathVector���ӹؿ�json�ļ��л�ȡ��·������
     * monster���ƶ��Ĺ���
     */
    void monsterMoveWithWayPoints(Vector<PointDelegate*> pathVector, Monster* monster);
    // ����HUD
    void updateHUD(float delta);
    // ������ֹ���
    void updateMonster(float delta);
    
    
    /**
     * ��⹥����Χ����ʾ���빥����Χ��false����ʾû�н��빥����Χ
     * monsterPoint �������ڵ������
     * turretPoint �������ڵ������
     * area ������Χ
     */
    bool checkPointInCircle(Point monsterPoint, Point turretPoint, int area);
    /**
     * �����ڵ�ʱ�����ڵ�����ת���򣬷��ر�ʾ��ת����ĸ�����
     * monsterPoint �������ڵ������
     * turret ����
     */
    float getTurretRotation(Point monsterPoint,Turret* turret);
    /**
     * �����ڵ��ƶ���������ڵ���Ҫ�ƶ�ʱ�䣬���ر�ʾʱ��ĸ�����
     * start �ڵ��ƶ��Ŀ�ʼ����
     * end �ڵ��ƶ��Ľ�������
     */
    float getBulletMoveTime(Point start, Point end);
    // ���������Ƿ��ϰ�
    bool getCollidable(Point position);
    // �������
    void detectionTurret(float delta);
    
    // �����ײ
    void collisionDetection(float delta);
    // ��⹫��
    void detectionPrincess(float delta);
    
    /**
     ��Ϸ������tag��0��ʾ��Ϸʧ�ܣ�1��ʾ��Ϸʤ��
     */
    void gameOver(int tag);
    CREATE_FUNC(Game);
};

#endif //__ProtectPrincessGame__GameScene__
    