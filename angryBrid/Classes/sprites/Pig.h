#ifndef __AngryBird__Pig
#define __AngryBird__Pig

#include "cocos2d.h"
#include "SpriteBase.h"
USING_NS_CC;

class Pig : public SpriteBase
{
public:
	static Pig* create(const std::string& filename)
	{
		Pig *sprite = new Pig();
        if (sprite && sprite->initWithFile(filename))
        {
            sprite->autorelease();
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
	}

	    void initWithData(float x,float y){
        // ����imageName
        this->setImageName("pig");
        // ����tag
        this->setTag(PIG_TAG);
        // ��������
        this->setPosition(x, y);
        // ��������ֵ
        this->setHP(1);
        this->setScale(2.0f/10);
        
        /**********�����������********************/
        // PhysicsBody::createEdgeBox����һ���ķ��εĸ��帽���ھ����ϡ�
        auto body = PhysicsBody::createBox(this->getContentSize()/5);
        /**����body״̬  falseΪ��̬ trueΪ��̬*/
        body->setDynamic(false);
        // ����������ײ��λ����
        body->setCategoryBitmask(0x01);
        body->setContactTestBitmask(0x01);
        body->setCollisionBitmask(0x01);
        body->setGroup(0);
        
        // ΪС����Ӹ�������
        this->setPhysicsBody(body);
    }
};



#endif //__AngryBird__Pig