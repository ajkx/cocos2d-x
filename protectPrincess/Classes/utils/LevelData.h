#ifndef __ProtectPrincess__LevelData__
#define __ProtectPrincess__LevelData__

#include "cocos2d.h"

USING_NS_CC;

// LevelData�࣬�����洢�ؿ�������Ϣ
// {"number": 1,"bg":"level1.png","data":"1.data"}
class LevelData : public Ref
{
protected:
    int _number; // �ؿ�����
    std::string _bg; // ����ͼƬ
    std::string _data;  // �ؿ��������ļ�
public:
    // ��̬��create����
    static LevelData* create(int number,std::string bg,std::string data)
    {
        LevelData *sprite = new LevelData();
        if (sprite)
        {
            sprite->autorelease();
            sprite->setNumber(number);
            sprite->setBg(bg);
            sprite->setData(data);
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }
    // setter��getter����
    void setNumber(int number){ _number = number; }
    int getNumber(){ return _number; }
    
    void setBg(std::string bg){ _bg = bg; }
    std::string getBg(){ return _bg; }
    
    void setData(std::string data){ _data = data; }
    std::string getData(){ return _data; }
    
};


#endif /* defined(__ProtectPrincess__LevelData__) */