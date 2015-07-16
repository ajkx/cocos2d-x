#ifndef __AngryBrid_ParticleManager
#define __AngryBrid_ParticleManager

#include "cocos2d.h"

USING_NS_CC;

//定义枚举类型 使用typedef可以直接使用别名
typedef enum{
	snow,	//雪花的粒子效果
	explosition		//小鸟爆破的粒子效果
}ParticleTypes;

class ParticleManager : public Ref
{
public:
	//获取单例对象
	static ParticleManager* getInstance();
	//获取指定type的粒子对象
	ParticleSystem* particleWithType(ParticleTypes type);
};

#endif // __AngryBrid_ParticleManager