#ifndef __RANKSCENE_H__
#define __RANKSCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class RankScene :public cocos2d::Layer{
private:
	
public:
	static cocos2d::Scene* createScene();
	bool init();

	void goHello();
	void clearRec();

	static float bestTime[5];


	CREATE_FUNC(RankScene);
};
#endif