//
//  GameScene.h
//  ParkourSample
//
//  Created by Ken on 13-10-10.
//
//

#ifndef ParkourSamplenGameScene_h
#define ParkourSample_GameScene_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameScenePlayLayer.h"
#include "MenuLayer.h"
USING_NS_CC;

using namespace extension;
class GameOverScene;


class GameScene : public cocos2d::CCScene
{
public:
    
    bool init();
	static GameScene * shareGameScene();
	static GameScene * newGameScene();
    GameScenePlayLayer* playLayer;

	MenuLayer* menuLayer;
};

#endif
