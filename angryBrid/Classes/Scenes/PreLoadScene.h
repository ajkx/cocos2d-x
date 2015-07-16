#ifndef __AngryBrid_PreLoadScene
#define __AngryBrid_PreLoadScene

#include "cocos2d.h"
#include "StartScene.h"

USING_NS_CC;

//��������
#define TITLE_FILE        "title.mp3"
#define LEVEL_SUCCESS_FILE        "level_success.mp3"
#define LEVEL_FAILED_FILE        "level_failed_pig_1.mp3"
#define BACKGROUND_FILE        "background.mp3"
#define BIRD_FLYING_FILE        "bird_flying_1.mp3"
#define ICE_FILE        "ice_1.mp3"
#define PIG_FILE        "pig_1.mp3"

class Preload : public cocos2d::Layer
{
private:
	//������
	ProgressTimer* progress;
	//���������´���
	float progressInterval;
	//��Դ����
	int sourceCount;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	//���½�����
	void load(float delta);

	void menuCloseCallback(Ref* pSender);

	CREATE_FUNC(Preload);
};

#endif // __AngryBrid_PreLoadScene