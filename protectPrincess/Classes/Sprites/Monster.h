#ifndef __ProtectPrincess__Monster__
#define __ProtectPrincess__Monster__

#include "cocos2d.h"
#include "SpriteBase.h"
#include "ui/CocosGUI.h"
using namespace ui;

// 怪物类
class Monster : public SpriteBase
{
private:
    int _lifeValue;  // 怪物的生命值
    LoadingBar* _HP; // 怪物血条
    float _HPInterval; // 怪物血条的更新量
public:
    
    // 静态的create函数
    static Monster* create(const std::string& filename)
    {
        Monster *sprite = new Monster();
        if (sprite && sprite->initWithFile(filename))
        {
            sprite->autorelease();
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }
    
    static Monster* createWithSpriteFrameName(const std::string& filename)
    {
        Monster *sprite = new Monster();
        if (sprite && sprite->initWithSpriteFrameName(filename))
        {
            sprite->autorelease();
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }
    // setter和getter函数
    void setLifeValue(int lifeValue){ _lifeValue = lifeValue; }
    int getLifeValue(){ return _lifeValue; }
    
    void setHP(LoadingBar* HP){
        _HP = HP;
    }
    LoadingBar* getHP(){
        return _HP;
    }
    void setHPInterval(float HPInterval){
        _HPInterval = HPInterval;
    }
    float getHpInterval(){
        return _HPInterval;
    }

};

#endif //__ProtectPrincess__Monster__