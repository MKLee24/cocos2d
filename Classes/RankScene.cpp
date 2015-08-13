#include "RankScene.h"
#include "HelloWorldScene.h"
#include "GameScene.h"
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"


using namespace CocosDenshion;
USING_NS_CC;

#pragma execution_character_set("utf-8")

float RankScene::bestTime[5] = { 999, 999, 999, 999, 999 };

Scene* RankScene::createScene()
{
	auto scene = Scene::create();
	auto layer = RankScene::create();
	scene->addChild(layer);
	return scene;
}

// on "init" you need to initialize your instance
bool RankScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	
	// 背景精灵
	auto bg1 = Sprite::create("background.jpg");
	//为适应屏幕进行缩放
	bg1->setScaleX((float)visibleSize.width / (float)bg1->getContentSize().width);
	bg1->setScaleY((float)visibleSize.height / (float)bg1->getContentSize().height);
	bg1->setPosition(Vec2(origin.x + visibleSize.width / 2, 0));
	bg1->setAnchorPoint(Vec2(0.5, 0));
	bg1->setTag(101);
	this->addChild(bg1, 0);

	// 预载入音乐 
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/rankScene.mp3");
	// 播放背景音乐
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/RankScene.mp3", true);

	// 根据UserDefault确定最短时间纪录
	if (!UserDefault::getInstance()->getBoolForKey("isExist"))
	{
		UserDefault::getInstance()->setBoolForKey("isExist", true);
		UserDefault::getInstance()->setFloatForKey("time1", 999);
		UserDefault::getInstance()->setFloatForKey("time2", 999);
		UserDefault::getInstance()->setFloatForKey("time3", 999);
		UserDefault::getInstance()->setFloatForKey("time4", 999);
		UserDefault::getInstance()->setFloatForKey("time5", 999);

	}
	bestTime[0] = UserDefault::getInstance()->getFloatForKey("time1");
	bestTime[1] = UserDefault::getInstance()->getFloatForKey("time2");
	bestTime[2] = UserDefault::getInstance()->getFloatForKey("time3");
	bestTime[3] = UserDefault::getInstance()->getFloatForKey("time4");
	bestTime[4] = UserDefault::getInstance()->getFloatForKey("time5");

	

	// 每次调用该函数时先清除一次显示的排行榜
	//this->removeChildByTag(9997);
	auto rankboard = Sprite::create("paper.png");
	rankboard->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 + 100));
	// 设置tag，方便每次调用该函数时先清除一次显示的排行榜
	//rankboard->setTag(9997);
	this->addChild(rankboard, 2);

	// 输出五关的最短时间
	for (int i = 0; i < 5; i++){	
		string sstr = "第";
		
		sstr += (char)(i + '1');
		sstr += "关最短时间: ";

		stringstream numb;
		numb << bestTime[i];
		if (bestTime[i] == 999) {
			sstr += "还未有记录";
		}
		else {
			sstr += numb.str();
			sstr += "秒";
		}
			
		auto lab = LabelTTF::create(sstr, "Arial", 30);
		lab->setColor(Color3B(0, 0, 0));
		lab->setPosition(Point(rankboard->getContentSize().width / 2, rankboard->getContentSize().height / 2 - i * 40));
		rankboard->addChild(lab);
		
		/*
		// 1.5秒内闪烁
		auto act = Blink::create(1.5, 10);
		rankboard->runAction(act);
		*/
	}

	
	// 回主页按钮和清除记录按钮
	auto goHome = MenuItemImage::create("return.png", "return2.png", CC_CALLBACK_0(RankScene::goHello, this));
	auto clearRecord = MenuItemImage::create("clean.png", "clean2.png", CC_CALLBACK_0(RankScene::clearRec, this));

	auto menu2 = Menu::create(goHome, clearRecord, NULL);
	menu2->alignItemsHorizontallyWithPadding(30);
	menu2->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 200);
	this->addChild(menu2, 3);

	return true;
}



// 返回首页
void RankScene::goHello() {
	auto scene = HelloWorld::createScene();
	auto gameScene = TransitionSlideInR::create(1.0f, scene);
	Director::getInstance()->replaceScene(gameScene);
}

// 清除记录
void RankScene::clearRec() {
	for (int i = 0; i < 5; i++) {
		bestTime[i] = 999;
	}

	UserDefault::getInstance()->setFloatForKey("time1", 999);
	UserDefault::getInstance()->setFloatForKey("time2", 999);
	UserDefault::getInstance()->setFloatForKey("time3", 999);
	UserDefault::getInstance()->setFloatForKey("time4", 999);
	UserDefault::getInstance()->setFloatForKey("time5", 999);

	// 刷新场景
	auto scene = RankScene::createScene();
	Director::getInstance()->replaceScene(scene);
}