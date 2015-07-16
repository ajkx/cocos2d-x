#ifndef __ProtectPrincess__PointDelegate__
#define __ProtectPrincess__PointDelegate__

#include "cocos2d.h"

// Point的代理类，继承cocos2d::Ref类
class PointDelegate : public cocos2d::Ref{
private:
    float _x;
    float _y;
public:
    static PointDelegate* create(float x, float y){
        PointDelegate* p = new PointDelegate();
        if(p && p->initPoint(x, y)){
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }
    bool initPoint(float x, float y){
        _x = x;
        _y = y;
        return true;
    }
    void setX(float x){ _x = x; }
    float getX(){ return _x; }
    
    void setY(float y){ _y = y; }
    float getY(){ return _y; }

};

#endif //__ProtectPrincess__PointDelegate__
