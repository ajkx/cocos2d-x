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
        // 设置imageName
        this->setImageName("ice");
        // 设置tag
        this->setTag(ICE_TAG);
        // 设置坐标
        this->setPosition(x, y);
        // 设置生命值
        this->setHP(10);
        this->setScale(2.0f/10);
        
        /**********添加物理引擎********************/
        // PhysicsBody::createEdgeBox创建一个四方形的刚体附加在精灵上。
        // 注意：如果box的size设置太大，则会出现冰块叠加时的缝隙
        auto body = PhysicsBody::createBox(this->getContentSize()/5.5);
        // 设置掩码
        body->setCategoryBitmask(0x01);
        body->setContactTestBitmask(0x01);
        body->setCollisionBitmask(0x01);
        body->setGroup(2);
        
        // 为冰块添加刚体属性
        this->setPhysicsBody(body);

    }
};

#endif //__AngryBird__Ice