
#ifndef __CuttingPhotos__
#define __CuttingPhotos__

#include <vector>
#include "cocos2d.h"

USING_NS_CC;

class cuttingPhotos : public cocos2d::Layer
{
    
public:
    static cuttingPhotos* create(const char* photo,unsigned int rows,unsigned int columns);
    
protected:
    
    unsigned int _rows;//行数
    unsigned int _columns;//列数
    unsigned int _sliceWidth;//格子宽度
    unsigned int _sliceHeight;//格子高度
    
    std::vector<std::vector<Sprite*>> _slices;//存储我们的切片
    
    virtual bool initWithPhoto(const char* photo,unsigned int rows,unsigned int columns);//初始化来自图片
    
    bool initSlice(const char* photo);//初始化切片数组
    
    void initTouchEventlistener();//初始化触摸事件
    
    void move(int x,int y);//移动指定位置的卡片
    
public:
    void restart();//重新开始（随机布置卡片）

    
};

#endif // __CuttingPhotos__
