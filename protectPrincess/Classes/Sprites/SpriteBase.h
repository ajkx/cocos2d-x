#ifndef __ProtectPrincess__SpriteBase__
#define __ProtectPrincess__SpriteBase__

#include "cocos2d.h"

USING_NS_CC;
// 扩展精灵类，增加通用的属性
class SpriteBase : public cocos2d::Sprite
{
private:
    std::string _name; // 精灵的名称
    int _gold; // 精灵的金币值
public:
    // 静态的create函数
    static SpriteBase* create(const std::string& filename)
    {
        SpriteBase *sprite = new SpriteBase();
        if (sprite && sprite->initWithFile(filename))
        {
            sprite->autorelease();
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }
    
    static SpriteBase* createWithSpriteFrameName(const std::string& filename)
    {
        SpriteBase *sprite = new SpriteBase();
        if (sprite && sprite->initWithSpriteFrameName(filename))
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
};

#endif //__ProtectPrincess__SpriteBase__