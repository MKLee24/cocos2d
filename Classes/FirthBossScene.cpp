#include "FirthBossScene.h"
#include "FirstBossScene.h"
#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "ChooseGuan.h"
#include "Man_1.h"
#include "Man_2.h"
#include "Man_3.h"
#include <iostream>
#include <string>
#include "Collision.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* FirthBossScene::createScene()
{
	// 	//创建物理世界的场景
	auto scene = Scene::createWithPhysics();
	auto layer = FirthBossScene::create();
	scene->addChild(layer);
	//获取魔法类型

	return scene;
}

bool FirthBossScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//可以解锁的关卡
	ChooseGuan::choosenum = 6;
	mantype = HelloWorld::getTypeMan();
	if (mantype == 1) {
		manfu = Man_1();
		man = Sprite::create("chooseman1.png");
	}
	else if (mantype == 2) {
		manfu = Man_2();
		man = Sprite::create("chooseman2.png");
	}
	else {
		manfu = Man_3();
		man = Sprite::create("chooseman3.png");
	}

	GameOverScene::win_num = 10;
	useKey = 0;     //初始化useKey
	score = 0;		//初始化Score
	Magic = 0;      //初始化人物魔法
	TypeWeapon = 0;       //初始化type
	blood_for_man = manfu.getBlood();    //初始化血量
	blood_for_boss = 20;  //初始化Boss的血量
	boss_move_up = true; //boss初始化向上移动
	rate = 5;   //鱼的移动速率初始化为5
	weapon_eff = manfu.getPower(); //初始化炸弹的威力
	increase_for_dis = 0; //魔法增加武器的距离
	increase_for_weapon = 0; //魔法增加武器的威力
	speed_for_man = manfu.getSpeed(); //初始化人的速度
	manscale = 1; //初始化人的大小
	po = 10; //AI的高低（表示概率）
	time_1 = 0.5;
	speed_for_water = 2;

	isPause = false;
	useBgm = true;

	// 预载入音乐、音效
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bgm.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("music/boom.mp3");
	// 播放背景音乐
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm.mp3", true);

	// 创建暂停按钮,返回按钮和控制音乐按钮.
	btn3 = MenuItemImage::create("pause.png", "pause.png", CC_CALLBACK_0(FirstBossScene::pauseGame, this));
	auto btn4 = MenuItemImage::create("home.png", "home.png", CC_CALLBACK_0(FirstBossScene::goHomePage, this));
	btn5 = MenuItemImage::create("musicoff.png", "musicon.png", CC_CALLBACK_0(FirstBossScene::soundUsing, this));
	btn3->setPosition(Vec2(visibleSize.width - btn3->getContentSize().width - 70, visibleSize.height - btn3->getContentSize().height));
	btn4->setPosition(Vec2(visibleSize.width - 2 * btn3->getContentSize().width - 70, visibleSize.height - btn3->getContentSize().height));
	btn5->setPosition(Vec2(visibleSize.width - 3 * btn3->getContentSize().width - 70, visibleSize.height - btn3->getContentSize().height));
	btn3->setAnchorPoint(Vec2(0, 0));
	btn4->setAnchorPoint(Vec2(0, 0));
	btn5->setAnchorPoint(Vec2(0, 0));
	auto menu3 = Menu::create(btn3, btn4, btn5, NULL);
	menu3->setPosition(Vec2::ZERO);
	this->addChild(menu3, 1);


	//背景精灵
	auto bg1 = Sprite::create("background.jpg");
	//为适应屏幕进行缩放
	bg1->setScaleX((float)visibleSize.width / (float)bg1->getContentSize().width);
	bg1->setScaleY((float)visibleSize.height / (float)bg1->getContentSize().height);
	bg1->setPosition(Vec2(origin.x + visibleSize.width / 2, 0));
	bg1->setAnchorPoint(Vec2(0.5, 0));
	bg1->setTag(101);
	this->addChild(bg1, 0);

	//创建人物及设置物理碰撞
	man->setPosition(30, visibleSize.height / 2);
	man->setTag(102);
	man->setAnchorPoint(Vec2(0, 0));
	auto manBody = PhysicsBody::createBox(man->getContentSize());
	manBody->setContactTestBitmask(0x0003);
	manBody->setCategoryBitmask(0x0001);
	manBody->setCollisionBitmask(0x0007);
	manBody->setGravityEnable(false);
	manBody->setDynamic(false);
	man->setPhysicsBody(manBody);
	this->addChild(man, 5);

	//label的集合
	createLabel();
	createWater(4, "water1.png", visibleSize.width / 2, 0, 10);
	createWater(1, "water2.png", visibleSize.width / 3, visibleSize.height / 3, 11);
	createWater(6, "water3.png", 100, visibleSize.height / 2, 12);

	//创建Boss
	bossCreate();

	//创建机器人
	createMachine();
	//武器一
	mSkillButton = SkillButton::createSkillButton(0.5, "boom_cd.png", "boom.png", "blood_blue_meitu_1.png");
	mSkillButton->setPosition(ccp(visibleSize.width - 50, 2 * mSkillButton->getContentSize().height + 50));
	addChild(mSkillButton);
	mSkillButton->setTouchSpriteProtocol(this);


	//boss技能
	this->schedule(schedule_selector(FirstBossScene::objectMove), 0.01);

	//boss移动
	this->schedule(schedule_selector(FirstBossScene::bossMove), 0.02);

	//鱼的创建
	this->schedule(schedule_selector(FirstBossScene::enemyCreate), 1);

	//炸弹移动
	this->schedule(schedule_selector(ThirdBossScene::bombsMove), 0.01);

	//判断水中的情况
	this->schedule(schedule_selector(ThirdBossScene::WaterMove), 0.01);

	//速率提升
	this->schedule(schedule_selector(FirstBossScene::setRate), 1);
	// 按键移动
	auto listenerkey = EventListenerKeyboard::create();
	listenerkey->onKeyPressed = CC_CALLBACK_2(FirstBossScene::keyPress, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerkey, this);

	auto listenerkey2 = EventListenerKeyboard::create();
	listenerkey2->onKeyReleased = CC_CALLBACK_2(FirstBossScene::keyRelease, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerkey2, this);

	schedule(schedule_selector(FirstBossScene::playerGo), 0.01f);

	schedule(schedule_selector(FirthBossScene::MachineMove), 0.01f);

	return true;
}

//创建机器人
void FirthBossScene::createMachine() {
	Machine = Sprite::create("Machine.png");
	Machine->setPosition(man->getContentSize().width, man->getPositionY());
	Machine->setTag(6);
	Machine->setAnchorPoint(Vec2(0, 0));
	sp_file.insert(pair<int, string>(6, "Machine.png"));

	//设置物理碰撞属性
	auto bombBody = PhysicsBody::createBox(Machine->getContentSize());
	bombBody->setContactTestBitmask(0x0003);
	bombBody->setCategoryBitmask(0x0002);
	bombBody->setCollisionBitmask(0x0001);
	bombBody->setGravityEnable(false);
	Machine->setPhysicsBody(bombBody);

	this->addChild(Machine, 5);
}

//机器人的移动
void FirthBossScene::MachineMove(float f) {
	Machine->setPosition(Vec2(man->getContentSize().width, man->getPositionY()));
}

//响应武器点击
void FirthBossScene::CMTouchSpriteSelectSprite(SkillButton *button){
	//做冷却结束的事件
	FourthBossScene::setBomb();
}