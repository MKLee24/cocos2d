#ifndef __FIRTHBOSSSCENE_H__
#define __FIRTHBOSSSCENE_H__

#include "FourthBossScene.h"

class FirthBossScene :public FourthBossScene {
	Sprite* Machine;
public:
	static cocos2d::Scene* createScene();
	bool init();
	//重写代理方法
	SkillButton* mSkillButton;
	void CMTouchSpriteSelectSprite(SkillButton *sprite);

	//创建机器人
	void createMachine();
	void MachineMove(float f);
	
	CREATE_FUNC(FirthBossScene);
};
#endif