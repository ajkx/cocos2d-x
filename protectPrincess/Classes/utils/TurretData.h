#ifndef __ProtectPrincess__TurretData__
#define __ProtectPrincess__TurretData__

#include "cocos2d.h"

USING_NS_CC;

// TurretData�࣬�����洢�ؿ���������Ϣ
// "turrets": [{"name":"bollte.png","gold":100}]
class TurretData : public Ref
{
private:
    std::string _name; // ��������
    int _gold;  // ��������������
    std::string _bulletName; // ����������ڵ�����
public:
    // ��̬��create����
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
    
    // setter��getter����
    void setName(std::string name){ _name = name; }
    std::string getName(){ return _name; }
    
    void setGold(int gold){ _gold = gold; }
    int getGold(){ return _gold; }
    
    void setBulletName(std::string bullet){ _bulletName = bullet; }
    std::string getBulletName(){ return _bulletName; }
    
};

#endif //__ProtectPrincess__TurretData__