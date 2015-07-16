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
			//粒子效果位置独立
			particleSystem->setPositionType(ParticleSystem::PositionType::FREE);
			//效果完成后自动删除
			particleSystem->setAutoRemoveOnFinish(true);
			break;
			}
		default:
			break;
	}
	return particleSystem;
}
