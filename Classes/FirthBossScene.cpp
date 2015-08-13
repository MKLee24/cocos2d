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
	// 	//������������ĳ���
	auto scene = Scene::createWithPhysics();
	auto layer = FirthBossScene::create();
	scene->addChild(layer);
	//��ȡħ������

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

	//���Խ����Ĺؿ�
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
	useKey = 0;     //��ʼ��useKey
	score = 0;		//��ʼ��Score
	Magic = 0;      //��ʼ������ħ��
	TypeWeapon = 0;       //��ʼ��type
	blood_for_man = manfu.getBlood();    //��ʼ��Ѫ��
	blood_for_boss = 20;  //��ʼ��Boss��Ѫ��
	boss_move_up = true; //boss��ʼ�������ƶ�
	rate = 5;   //����ƶ����ʳ�ʼ��Ϊ5
	weapon_eff = manfu.getPower(); //��ʼ��ը��������
	increase_for_dis = 0; //ħ�����������ľ���
	increase_for_weapon = 0; //ħ����������������
	speed_for_man = manfu.getSpeed(); //��ʼ���˵��ٶ�
	manscale = 1; //��ʼ���˵Ĵ�С
	po = 10; //AI�ĸߵͣ���ʾ���ʣ�
	time_1 = 0.5;
	speed_for_water = 2;

	isPause = false;
	useBgm = true;

	// Ԥ�������֡���Ч
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bgm.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("music/boom.mp3");
	// ���ű�������
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm.mp3", true);

	// ������ͣ��ť,���ذ�ť�Ϳ������ְ�ť.
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


	//��������
	auto bg1 = Sprite::create("background.jpg");
	//Ϊ��Ӧ��Ļ��������
	bg1->setScaleX((float)visibleSize.width / (float)bg1->getContentSize().width);
	bg1->setScaleY((float)visibleSize.height / (float)bg1->getContentSize().height);
	bg1->setPosition(Vec2(origin.x + visibleSize.width / 2, 0));
	bg1->setAnchorPoint(Vec2(0.5, 0));
	bg1->setTag(101);
	this->addChild(bg1, 0);

	//�������Ｐ����������ײ
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

	//label�ļ���
	createLabel();
	createWater(4, "water1.png", visibleSize.width / 2, 0, 10);
	createWater(1, "water2.png", visibleSize.width / 3, visibleSize.height / 3, 11);
	createWater(6, "water3.png", 100, visibleSize.height / 2, 12);

	//����Boss
	bossCreate();

	//����������
	createMachine();
	//����һ
	mSkillButton = SkillButton::createSkillButton(0.5, "boom_cd.png", "boom.png", "blood_blue_meitu_1.png");
	mSkillButton->setPosition(ccp(visibleSize.width - 50, 2 * mSkillButton->getContentSize().height + 50));
	addChild(mSkillButton);
	mSkillButton->setTouchSpriteProtocol(this);


	//boss����
	this->schedule(schedule_selector(FirstBossScene::objectMove), 0.01);

	//boss�ƶ�
	this->schedule(schedule_selector(FirstBossScene::bossMove), 0.02);

	//��Ĵ���
	this->schedule(schedule_selector(FirstBossScene::enemyCreate), 1);

	//ը���ƶ�
	this->schedule(schedule_selector(ThirdBossScene::bombsMove), 0.01);

	//�ж�ˮ�е����
	this->schedule(schedule_selector(ThirdBossScene::WaterMove), 0.01);

	//��������
	this->schedule(schedule_selector(FirstBossScene::setRate), 1);
	// �����ƶ�
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

//����������
void FirthBossScene::createMachine() {
	Machine = Sprite::create("Machine.png");
	Machine->setPosition(man->getContentSize().width, man->getPositionY());
	Machine->setTag(6);
	Machine->setAnchorPoint(Vec2(0, 0));
	sp_file.insert(pair<int, string>(6, "Machine.png"));

	//����������ײ����
	auto bombBody = PhysicsBody::createBox(Machine->getContentSize());
	bombBody->setContactTestBitmask(0x0003);
	bombBody->setCategoryBitmask(0x0002);
	bombBody->setCollisionBitmask(0x0001);
	bombBody->setGravityEnable(false);
	Machine->setPhysicsBody(bombBody);

	this->addChild(Machine, 5);
}

//�����˵��ƶ�
void FirthBossScene::MachineMove(float f) {
	Machine->setPosition(Vec2(man->getContentSize().width, man->getPositionY()));
}

//��Ӧ�������
void FirthBossScene::CMTouchSpriteSelectSprite(SkillButton *button){
	//����ȴ�������¼�
	FourthBossScene::setBomb();
}