#ifndef __AngryBird__Bird
#define __AngryBird__Bird

#include "cocos2d.h"
#include "SpriteBase.h"

USING_NS_CC;

class Bird : public SpriteBase
{
protected:
	// 小鸟是否准备好
	bool isReady;
	// 小鸟是否在飞
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
        // 设置imageName
        this->setImageName("bird");
        // 设置tag
        this->setTag(BIRD_TAG);
        // 设置坐标
        this->setPosition(x, y);
        // 设置生命值
        this->setHP(1);
        this->setScale(0.3f);

		auto body = PhysicsBody::createCircle(this->getContentSize().width/4-8);
		// 因为小鸟是圆形，所以首先设置成静态物体，否则小鸟会在地上滚动
		body->setDynamic(false);
		        /**
         setCategoryBitMask函数设置当前PhysicsBody的位掩码
         场景中的每个物理身体可以分配给多达32个不同的类别,每一个对应于一个位的位掩码。
         在游戏中定义使用掩码值，
         结合collisionBitMask和contactTestBitMask属性,当游戏中定义的物理身体彼此交互时可以进行检测通知。
         
         setContactTestBitMask函数表示碰撞后会造成影响的PhysicsBody的位掩码
         
         setCollisionBitMask函数表示可以有碰撞关系的PhysicsBody的位掩码
         位掩码的类型为unsigned int，用16进制数表示，当两个物理身体互相接触,可能发生碰撞。
         判断两个sprite能否碰撞，
         用当前sprite的categoryBitMask与对方sprite的collisionBitMask做按位与运算，
         运算结果为1则可以发生碰撞，为0则不可。
         */
        body->setCategoryBitmask(0x01);
        body->setContactTestBitmask(0x01);
        body->setCollisionBitmask(0x01);
        body->setGroup(0);

        // 为小鸟添加刚体属性
        this->setPhysicsBody(body);
	}
	void setReady(bool _isReady){ isReady = _isReady;}
	bool getReady(){ return isReady;}

	void setFly(bool _isFly){ isFly = _isFly;}
	bool getFly(){ return isFly;}
};

#endif //__AngryBird__Bird__