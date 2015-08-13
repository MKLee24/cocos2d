#ifndef __WINSCENE_H__
#define __WINSCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class winScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuMagic1Callback();
	void menuMagic2Callback();

	CREATE_FUNC(winScene);
};

#endif // __WINSCENE_H__