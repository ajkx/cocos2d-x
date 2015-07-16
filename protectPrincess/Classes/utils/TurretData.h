#ifndef __ProtectPrincess__TurretData__
#define __ProtectPrincess__TurretData__

#include "cocos2d.h"

USING_NS_CC;

// TurretData类，用来存储关卡的炮塔信息
// "turrets": [{"name":"bollte.png","gold":100}]
class TurretData : public Ref
{
private:
    std::string _name; // 炮塔名称
    int _gold;  // 建造炮塔所需金币
    std::string _bulletName; // 炮塔发射的炮弹名称
public:
    // 静态的create函数
    static TurretData* create()
    {
        TurretData *sprite = new TurretData();
        if (sprite)
        {
            sprite->autorelease();
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }
    
    // setter和getter函数
    void setName(std::string name){ _name = name; }
    std::string getName(){ return _name; }
    
    void setGold(int gold){ _gold = gold; }
    int getGold(){ return _gold; }
    
    void setBulletName(std::string bullet){ _bulletName = bullet; }
    std::string getBulletName(){ return _bulletName; }
    
};

#endif //__ProtectPrincess__TurretData__