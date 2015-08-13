#include "winScene.h"
#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"


using namespace CocosDenshion;
USING_NS_CC;


Scene* winScene::createScene()
{
	auto scene = Scene::create();
	auto layer = winScene::create();
	scene->addChild(layer);

	return scene;
}

// on "init" you need to initialize your instance
bool winScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//±³¾°¾«Áé
	auto bg1 = Sprite::create("win_bg1.png");
	//ÎªÊÊÓ¦ÆÁÄ»½øÐÐËõ·Å
	bg1->setScaleX((float)visibleSize.width / (float)bg1->getContentSize().width);
	bg1->setScaleY((float)visibleSize.height / (float)bg1->getContentSize().height);
	bg1->setPosition(Vec2(origin.x + visibleSize.width / 2, 0));
	bg1->setAnchorPoint(Vec2(0.5, 0));
	bg1->setTag(101);
	this->addChild(bg1, 0);

	// Ô¤ÔØÈëÒôÀÖ 
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/win1.mp3");
	// ²¥·Å±³¾°ÒôÀÖ
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/win1.mp3", true);

	auto Magic1Item = MenuItemImage::create("return.png", "return2.png",
		CC_CALLBACK_0(winScene::menuMagic1Callback, this));
	Magic1Item->setPosition(Vec2(visibleSize.width / 2 - Magic1Item->getContentSize().width / 2, 120));
	Magic1Item->setAnchorPoint(Vec2(0, 0));
	auto Magic2Item = MenuItemImage::create("exit.png", "exit2.png",
		CC_CALLBACK_0(winScene::menuMagic2Callback, this));
	Magic2Item->setPosition(Vec2(visibleSize.width / 2 - Magic2Item->getContentSize().width / 2, 120 - Magic1Item->getContentSize().height));
	Magic2Item->setAnchorPoint(Vec2(0, 0));
	auto menu = Menu::create(Magic1Item, Magic2Item, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}

void winScene::menuMagic1Callback() {
	GameOverScene::win_num = 1;
	auto scene = HelloWorld::createScene();
	auto gameScene = TransitionSlideInR::create(1.0f, scene);
	Director::getInstance()->replaceScene(gameScene);
}

void winScene::menuMagic2Callback() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}