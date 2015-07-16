#ifndef __AngryBrid_ParticleManager
#define __AngryBrid_ParticleManager

#include "cocos2d.h"

USING_NS_CC;

//����ö������ ʹ��typedef����ֱ��ʹ�ñ���
typedef enum{
	snow,	//ѩ��������Ч��
	explosition		//С���Ƶ�����Ч��
}ParticleTypes;

class ParticleManager : public Ref
{
public:
	//��ȡ��������
	static ParticleManager* getInstance();
	//��ȡָ��type�����Ӷ���
	ParticleSystem* particleWithType(ParticleTypes type);
};

#endif // __AngryBrid_ParticleManager