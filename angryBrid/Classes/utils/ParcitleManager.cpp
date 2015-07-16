#include "ParcitleManager.h"

static ParticleManager *s_SharedParticleManager = nullptr;

ParticleManager* ParticleManager::getInstance()
{
	if(!s_SharedParticleManager){
		s_SharedParticleManager = new ParticleManager();
	}
	return s_SharedParticleManager;
}

ParticleSystem* ParticleManager::particleWithType(ParticleTypes type){
	ParticleSystem* particleSystem = nullptr;
	switch(type){
		case snow:
			{
			particleSystem = ParticleSnow::create();
			Texture2D* texture2D = Director::getInstance()->getTextureCache()->addImage("snow.png");
			particleSystem->setTexture(texture2D);
			break;
			}
		case explosition:
			{
			particleSystem = ParticleSystemQuad::create("bird-explosition.plist");
			//����Ч��λ�ö���
			particleSystem->setPositionType(ParticleSystem::PositionType::FREE);
			//Ч����ɺ��Զ�ɾ��
			particleSystem->setAutoRemoveOnFinish(true);
			break;
			}
		default:
			break;
	}
	return particleSystem;
}
