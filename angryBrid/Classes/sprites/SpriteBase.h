#ifndef __AngryBirdGame__SpriteBase
#define __AngryBirdGame__SpriteBase

#include "cocos2d.h"

USING_NS_CC;

// С��
#define BIRD_TAG 1
// С��
#define PIG_TAG 2
// ����
#define ICE_TAG 3
// ǽ��
#define WALL_TAG 4

class SpriteBase : public cocos2d::Sprite
{
private:
	float HP;
	std::string imageName;
public:
	static SpriteBase* create(const std::string& filename)
	{
		SpriteBase *sprite = new SpriteBase();
		if(sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	void setImageName(std::string _imageName){ imageName = _imageName; }
    std::string getImageName(){ return imageName; }
    
    void setHP(float _hp){ HP = _hp; }
    float getHP(){ return HP; }
};

#endif //__AngryBirdGame__SpriteBase