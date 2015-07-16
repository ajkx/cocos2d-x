#ifndef __ProtectPrincess__Turret__
#define __ProtectPrincess__Turret__

#include "cocos2d.h"
#include "SpriteBase.h"
#include "Bullet.h"

// ������
class Turret : public SpriteBase
{
private:
    std::string _bulletName; // ����������ڵ�����
    bool _select ; // �����Ƿ񱻰���
    Bullet* _bullet ; // �����������ڵ�����
public:
    
    // ��̬��create����
    static Turret* create(const std::string& filename)
    {
        Turret *sprite = new Turret();
        if (sprite && sprite->initWithFile(filename))
        {
            sprite->autorelease();
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }
    
    static Turret* createWithSpriteFrameName(const std::string& filename)
    {
        Turret *sprite = new Turret();
        if (sprite && sprite->initWithSpriteFrameName(filename))
        {
            sprite->autorelease();
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }
    // setter��getter����
    void setBulletName(std::string bulletName){ _bulletName = bulletName; }
    std::string getBulletName(){ return _bulletName; }
    
    void setSelect(bool select){ _select = select; }
    bool isSelect(){ return _select; }
    
    void setBullet(Bullet* bullet){ _bullet = bullet; }
    Bullet* getBullet(){ return _bullet; }

};

#endif //__ProtectPrincess__Turret__