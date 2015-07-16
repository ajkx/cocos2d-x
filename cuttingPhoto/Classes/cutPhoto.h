
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
    
    unsigned int _rows;//����
    unsigned int _columns;//����
    unsigned int _sliceWidth;//���ӿ��
    unsigned int _sliceHeight;//���Ӹ߶�
    
    std::vector<std::vector<Sprite*>> _slices;//�洢���ǵ���Ƭ
    
    virtual bool initWithPhoto(const char* photo,unsigned int rows,unsigned int columns);//��ʼ������ͼƬ
    
    bool initSlice(const char* photo);//��ʼ����Ƭ����
    
    void initTouchEventlistener();//��ʼ�������¼�
    
    void move(int x,int y);//�ƶ�ָ��λ�õĿ�Ƭ
    
public:
    void restart();//���¿�ʼ��������ÿ�Ƭ��

    
};

#endif // __CuttingPhotos__
