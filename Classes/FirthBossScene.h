#ifndef __FIRTHBOSSSCENE_H__
#define __FIRTHBOSSSCENE_H__

#include "FourthBossScene.h"

class FirthBossScene :public FourthBossScene {
	Sprite* Machine;
public:
	static cocos2d::Scene* createScene();
	bool init();
	//��д������
	SkillButton* mSkillButton;
	void CMTouchSpriteSelectSprite(SkillButton *sprite);

	//����������
	void createMachine();
	void MachineMove(float f);
	
	CREATE_FUNC(FirthBossScene);
};
#endif