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
        // 设置imageName
        this->setImageName("pig");
        // 设置tag
        this->setTag(PIG_TAG);
        // 设置坐标
        this->setPosition(x, y);
        // 设置生命值
        this->setHP(1);
        this->setScale(2.0f/10);
        
        /**********添加物理引擎********************/
        // PhysicsBody::createEdgeBox创建一个四方形的刚体附加在精灵上。
        auto body = PhysicsBody::createBox(this->getContentSize()/5);
        /**设置body状态  false为静态 true为动态*/
        body->setDynamic(false);
        // 设置物理碰撞的位掩码
        body->setCategoryBitmask(0x01);
        body->setContactTestBitmask(0x01);
        body->setCollisionBitmask(0x01);
        body->setGroup(0);
        
        // 为小猪添加刚体属性
        this->setPhysicsBody(body);
    }
};



#endif //__AngryBird__Pig