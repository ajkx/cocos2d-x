#ifndef __ProtectPrincess__SpriteBase__
#define __ProtectPrincess__SpriteBase__

#include "cocos2d.h"

USING_NS_CC;
// ��չ�����࣬����ͨ�õ�����
class SpriteBase : public cocos2d::Sprite
{
private:
    std::string _name; // ���������
    int _gold; // ����Ľ��ֵ
public:
    // ��̬��create����
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
    
    // setter��getter����
    void setName(std::string name){ _name = name; }
    std::string getName(){ return _name; }
    
    void setGold(int gold){ _gold = gold; }
    int getGold(){ return _gold; }
};

#endif //__ProtectPrincess__SpriteBase__