//
//  GameScenePlayLayer.cpp
//  ChenXiaoGe
//
//  Created by Ken on 13-10-10.
//
//  Modified by binss on 2014-1-16
//	For Coco2d-x final homework

#include "GameScenePlayLayer.h"

#define ANIME_RUN 0
#define ANIME_JUMP 0

bool GameScenePlayLayer::init()
{
	crashArray = CCArray::create();
	crashArray->retain();

    winSize = CCDirector::sharedDirector()->getWinSize();

	this->setTouchEnabled(true);
	this->setTouchMode(kCCTouchesOneByOne);


	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("pictures/background.plist");
	//����һ������������ڵ㣬CCSpriteBatchNode�������ܵر����������еĺ��ӽ�㣬��ͨ��һ��OpenGL ES call����Ⱦ��Щ����,������Ⱦ�ٶ�
	CCSpriteBatchNode *spriteBatch= CCSpriteBatchNode::create("pictures/background.png");  
	this->addChild(spriteBatch);


	background_spring = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("background-spring.png");
	background_summer = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("background-summer.png");
	background_autumn = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("background-autumn.png");
	background_winter = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("background-winter.png");
	ground_spring = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("soilGround.png");
	ground_summer = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("greenGround.png");
	ground_autumn = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("mapleGround.png");
	ground_winter = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("snowGround.png");


	backgroundInit();

	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("animations/ChenXiaoGeJumping0.png",
		"animations/ChenXiaoGeJumping0.plist","animations/ChenXiaoGeJumping.ExportJson");

	this->running();
	actionNum = ACTION_RUN;
	score = 0;

	this->schedule(schedule_selector(GameScenePlayLayer::createCoin),0.8f);
	this->schedule(schedule_selector(GameScenePlayLayer::changeSeason),10);
	this->scheduleUpdate();   
	
    return true;
}

void GameScenePlayLayer::createCoin(float dt)
{
	char* pic;
	int tag;
	int randomType = CCRANDOM_0_1() * 100 + 1;

	if (randomType < 0)
	{
		tag = 1;
		pic = "pictures/cloverSliverCoin.png";     //1ʱΪ����
	}
	else if(randomType < 10)
	{
		pic = "pictures/cloverGlodenCoin.png";	   //2ʱΪ���
		tag = 2;
	}
	else
	{
		pic = "pictures/roadBlock.png";			  //3ʱΪ·��
		tag = 3;
	}
	
	CCSprite *object = CCSprite::create(pic);
	object->setScale(0.2f);
	//CCSprite *object = CCSprite::createWithSpriteFrameName(pic);
	object->setTag(tag);
	object->setPosition(ccp(object->getContentSize().width/2 + 480 ,object->getContentSize().height/2 + 25));
	crashArray->addObject(object);
	this->addChild(object);
	/*
	CCMoveBy *action = CCMoveBy::create(6,ccp(-480 - object->getContentSize().width / 2,0));               //the second parameter is offset

	CCCallFuncO *callback = CCCallFuncO::create(this,callfuncO_selector(GameScenePlayLayer::rubbishCollection), object);
	CCSequence *sequence = CCSequence::create(action,callback,NULL);   //do action and then callback(destroy)
	object->runAction(sequence);
	*/
}

void GameScenePlayLayer::rubbishCollection(CCObject *object)               //destroy the object which is out of view
{
	CCSprite *sprite = (CCSprite*)object;
	if (sprite->getTag() < 4 )
	{
		crashArray->removeObject(sprite);
		//sprite->removeAllChildrenWithCleanup(true);
		this->removeChild(sprite,true);
	}
	else if(sprite->getTag() == 5)
	{
		sprite->removeAllChildrenWithCleanup(true);
		actionNum = ACTION_RUN;
		chenxiaogeArmature->setVisible(true);
	}
}



bool GameScenePlayLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
   	runJump();
	return true;
}

void GameScenePlayLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}

void GameScenePlayLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}

void GameScenePlayLayer::running()
{
	chenxiaogeArmature = CCArmature::create("ChenXiaoGeRunning");
	chenxiaogeArmature->setScale(0.3f);
	chenxiaogeArmature->getAnimation()->play("running");
	chenxiaogeArmature->getAnimation()->setSpeedScale(2.0f);
	chenxiaogeArmature->setAnchorPoint(ccp(0.5,0));
	chenxiaogeArmature->setPosition((chenxiaogeArmature->getContentSize().width)*0.3 + 20, ground->getContentSize().height - 20);
	chenxiaogeArmature->setTag(5);
	addChild(chenxiaogeArmature,5);
	
	actionNum = ACTION_RUN;
	

}

void GameScenePlayLayer::runJump()
{  
	CCLOG("jump :%i",actionNum);
	if(actionNum == ACTION_RUN)
	{
		chenxiaogeArmature->setVisible(false);
		CCArmature *armature = NULL;
		armature = CCArmature::create("ChenXiaoGeJumping");
		armature->setScale(0.3f);
		armature->getAnimation()->setSpeedScale(2.0f);
		armature->setAnchorPoint(ccp(0.5,0));
		armature->getAnimation()->play("running");
		armature->setTag(5);
		armature->setPosition((armature->getContentSize().width)*0.3 + 20, ground->getContentSize().height - 20);
		addChild(armature,8);
		actionNum = ACTION_RUN_JUMP;
		
		CCActionInterval *jump = CCJumpBy::create(1.0f,ccp(0,0), 50, 1);    //��������Ծʱ�䣬���λ�ã����߶ȣ�����
		CCCallFuncO *callback = CCCallFuncO::create(this,callfuncO_selector(GameScenePlayLayer::rubbishCollection),armature);
		CCSequence *sequence = CCSequence::create(jump,callback,NULL);
		armature->runAction(sequence);  
	}
}


void GameScenePlayLayer::changeSeason(float dt)
{
	season++;
	if(season >4)
		season = 1;
	switch (season)
	{
	case 1:
		{
			background->setDisplayFrame(background_spring);
			backgroundCopy->setDisplayFrame(background_spring);
			ground->setDisplayFrame(ground_spring);
			groundCopy->setDisplayFrame(ground_spring);
			particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("pictures/sakura.png"));
			particle2->setTexture(CCTextureCache::sharedTextureCache()->addImage("pictures/sakura2.png"));
			break;
		}
	case 2:
		{
			background->setDisplayFrame(background_summer);
			backgroundCopy->setDisplayFrame(background_summer);
			ground->setDisplayFrame(ground_summer);
			groundCopy->setDisplayFrame(ground_summer);
			particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("pictures/leaf.png"));
			particle2->setTexture(CCTextureCache::sharedTextureCache()->addImage("pictures/leaf2.png"));
			break;
		}
	case 3:
		{
			background->setDisplayFrame(background_autumn);
			backgroundCopy->setDisplayFrame(background_autumn);
			ground->setDisplayFrame(ground_autumn);
			groundCopy->setDisplayFrame(ground_autumn);
			particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("pictures/MapleLeaves1.png"));
			particle2->setTexture(CCTextureCache::sharedTextureCache()->addImage("pictures/MapleLeaves2.png"));
			break;
		}
	case 4:
		{
			background->setDisplayFrame(background_winter);
			backgroundCopy->setDisplayFrame(background_winter);
			ground->setDisplayFrame(ground_winter);
			groundCopy->setDisplayFrame(ground_winter);
			particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("pictures/snow.png"));
			particle2->setTexture(CCTextureCache::sharedTextureCache()->addImage("pictures/snow.png"));
			break;
		}
		
	}
}

void GameScenePlayLayer::backgroundInit()
{
	season = 1;     //�趨����
	//�������Ч��
	particle = CCParticleSnow::create();
	particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("pictures/sakura.png"));
	addChild(particle,5);
	particle2 = CCParticleSnow::create();
	particle2->setTexture(CCTextureCache::sharedTextureCache()->addImage("pictures/sakura2.png"));
	addChild(particle2,6);
	float winWidth = winSize.width;
	float winHeight = winSize.height;
    
	//��ʼ������
	background = CCSprite::createWithSpriteFrameName("background-spring.png");
	backgroundCopy = CCSprite::createWithSpriteFrameName("background-spring.png");
	shop = CCSprite::create("pictures/shop.png");
	sun = CCSprite::create("pictures/sun.png");
	ground = CCSprite::createWithSpriteFrameName("soilGround.png");;
    groundCopy = CCSprite::createWithSpriteFrameName("soilGround.png");

	background->setAnchorPoint(ccp(0,0));
	backgroundCopy->setAnchorPoint(ccp(0,0));
	shop->setAnchorPoint(ccp(0,0));
	sun->setAnchorPoint(ccp(0,0));
	ground->setAnchorPoint(ccp(0,0));
	groundCopy->setAnchorPoint(ccp(0,0));

    resetBackground();
    sun->setPosition(ccp(winWidth/2, winHeight*0.7));

	addChild(background,-2);			//������һ��sprite�����ȼ�������coin���ذ��ڵ������
	addChild(backgroundCopy,-2);
	addChild(shop,1);
	addChild(sun,6);
	addChild(ground,-1);
	addChild(groundCopy,-1);
}


void GameScenePlayLayer::update(float dt)
{
	CCObject *obj;
	CCARRAY_FOREACH(crashArray,obj)
	{
		CCSprite *object = (CCSprite*)obj;
		object->setPositionX(object->getPosition().x-1);

		if(object->getPosition().x >= 49 && object->getPosition().x <= 51 && actionNum == ACTION_RUN)
		{
			if(object->getTag() == 3)  //�����ж�
			{
				CCLOG("gameover");
				gameOver();
				continue;
			}
			if(object->getTag() == 1)  //����
			{
				score ++;
				rubbishCollection(object);
				CCLOG("%i",score);
				continue;
			}
			if(object->getTag() == 2)  //���
			{
				score += 10;
				CCLOG("%i",score);
				rubbishCollection(object);
				continue;
			}
		}
		if(object->getPosition().x < 0)
			rubbishCollection(object);
	}

    if(backgroundCopy ->getPosition().x>=0)
    {
		background->setPosition(ccp(background->getPosition().x-1, 0));
		backgroundCopy->setPosition(ccp(backgroundCopy->getPosition().x-1, 0));
		shop->setPosition(ccp(shop->getPosition().x-1.5, shop->getPosition().y));
		ground->setPosition(ccp(ground->getPosition().x-1, 0));
		groundCopy->setPosition(ccp(groundCopy->getPosition().x-1, 0));
	 }
	else
	{
		resetBackground();
	}
}

void GameScenePlayLayer::resetBackground()
{
	float winWidth = winSize.width;
	float winHeight = winSize.height;
    background->setPosition(ccp(0,0));
	backgroundCopy->setPosition(ccp(background->getPosition().x + background->getContentSize().width -1, 0)); //-1����ʹ��SpriteFrame�����ı߿�
	ground->setPosition(ccp(0,0));
	groundCopy->setPosition(ccp(ground->getPosition().x + ground->getContentSize().width -1, 0));
	shop->setPosition(ccp(winWidth, ground->getContentSize().height));
}

void GameScenePlayLayer::gameOver()
{
	//GameOverScene *scene = GameOverScene::create();
	//CCDirector::sharedDirector()->replaceScene(scene);
}