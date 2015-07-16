#ifndef __AngryBird__Ice
#define __AngryBird__Ice

#include "cocos2d.h"
#include "SpriteBase.h"

USING_NS_CC;

class Ice : public SpriteBase
{
public:
	static Ice* create(const std::string& filename)
	{
        Ice *sprite = new Ice();
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
        this->setImageName("ice");
        // ����tag
        this->setTag(ICE_TAG);
        // ��������
        this->setPosition(x, y);
        // ��������ֵ
        this->setHP(10);
        this->setScale(2.0f/10);
        
        /**********�����������********************/
        // PhysicsBody::createEdgeBox����һ���ķ��εĸ��帽���ھ����ϡ�
        // ע�⣺���box��size����̫�������ֱ������ʱ�ķ�϶
        auto body = PhysicsBody::createBox(this->getContentSize()/5.5);
        // ��������
        body->setCategoryBitmask(0x01);
        body->setContactTestBitmask(0x01);
        body->setCollisionBitmask(0x01);
        body->setGroup(2);
        
        // Ϊ������Ӹ�������
        this->setPhysicsBody(body);

    }
};

#endif //__AngryBird__Ice