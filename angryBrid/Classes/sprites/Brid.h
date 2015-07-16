#ifndef __AngryBird__Bird
#define __AngryBird__Bird

#include "cocos2d.h"
#include "SpriteBase.h"

USING_NS_CC;

class Bird : public SpriteBase
{
protected:
	// С���Ƿ�׼����
	bool isReady;
	// С���Ƿ��ڷ�
	bool isFly;
public:
	static Bird* create(const std::string& filename)
	{
		Bird *sprite = new Bird();
        if (sprite && sprite->initWithFile(filename))
        {
            sprite->autorelease();
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
	}

	void initWithData(float x,float y)
	{
        // ����imageName
        this->setImageName("bird");
        // ����tag
        this->setTag(BIRD_TAG);
        // ��������
        this->setPosition(x, y);
        // ��������ֵ
        this->setHP(1);
        this->setScale(0.3f);

		auto body = PhysicsBody::createCircle(this->getContentSize().width/4-8);
		// ��ΪС����Բ�Σ������������óɾ�̬���壬����С����ڵ��Ϲ���
		body->setDynamic(false);
		        /**
         setCategoryBitMask�������õ�ǰPhysicsBody��λ����
         �����е�ÿ������������Է�������32����ͬ�����,ÿһ����Ӧ��һ��λ��λ���롣
         ����Ϸ�ж���ʹ������ֵ��
         ���collisionBitMask��contactTestBitMask����,����Ϸ�ж������������˴˽���ʱ���Խ��м��֪ͨ��
         
         setContactTestBitMask������ʾ��ײ������Ӱ���PhysicsBody��λ����
         
         setCollisionBitMask������ʾ��������ײ��ϵ��PhysicsBody��λ����
         λ���������Ϊunsigned int����16��������ʾ���������������廥��Ӵ�,���ܷ�����ײ��
         �ж�����sprite�ܷ���ײ��
         �õ�ǰsprite��categoryBitMask��Է�sprite��collisionBitMask����λ�����㣬
         ������Ϊ1����Է�����ײ��Ϊ0�򲻿ɡ�
         */
        body->setCategoryBitmask(0x01);
        body->setContactTestBitmask(0x01);
        body->setCollisionBitmask(0x01);
        body->setGroup(0);

        // ΪС����Ӹ�������
        this->setPhysicsBody(body);
	}
	void setReady(bool _isReady){ isReady = _isReady;}
	bool getReady(){ return isReady;}

	void setFly(bool _isFly){ isFly = _isFly;}
	bool getFly(){ return isFly;}
};

#endif //__AngryBird__Bird__