#ifndef __ProtectPrincess__MonsterData__
#define __ProtectPrincess__MonsterData__

#include "cocos2d.h"

USING_NS_CC;

// MonsterData类，用来存储关卡的怪物信息
// "monsters": [{"name":"pest.png","lifeValue":2,"gold":15}]
class MonsterData : public Ref
{
private:
    std::string _name; // 怪物名称
    int _lifeValue;  // 怪物的生命值
    int _gold; // 消灭怪物获得的金币
public:
    // 静态的create函数
    static MonsterData* create()
    {
        MonsterData *sprite = new MonsterData();
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
    
    void setLifeValue(int lifeValue){ _lifeValue = lifeValue; }
    int getLifeValue(){ return _lifeValue; }
    
    void setGold(int gold){ _gold = gold; }
    int getGold(){ return _gold; }
    
};


#endif //ProtectPrincess__MonsterData__
