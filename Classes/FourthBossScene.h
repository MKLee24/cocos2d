#ifndef __FOURTHBOSSSCENE_H__
#define __FOURTHBOSSSCENE_H__

#include "ThirdBossScene.h"

class FourthBossScene :public ThirdBossScene {
public:
	int po;//AI的高低
	static cocos2d::Scene* createScene();
	bool init();
	void setBomb();
	//重写代理方法
	SkillButton* mSkillButton;
	void CMTouchSpriteSelectSprite(SkillButton *sprite);
	CREATE_FUNC(FourthBossScene);
};
#endif