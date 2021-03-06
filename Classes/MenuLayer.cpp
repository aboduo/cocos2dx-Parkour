#include "MenuLayer.h"
#include "GameScene.h"
bool MenuLayer::init(int broodBarPercent, const char *value)
{
	if(UILayer::init())
	{
		this->setTouchEnabled(true);
		this->setTouchMode(kCCTouchesOneByOne);

		Widget * myWidget =  dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile("UI/Menu/Menu_1.json"));
		this->addWidget(myWidget);
		UIButton* settingBtn = dynamic_cast<UIButton*>(this->getWidgetByName("setBt"));
		settingBtn->setTouchEnabled(true);
		settingBtn->setPressedActionEnabled(true);        
		settingBtn->addTouchEventListener(this, toucheventselector(MenuLayer::settingBtnCallback));


		scoreLabel = dynamic_cast<UILabelAtlas*>(this->getWidgetByName("score"));
		setScore(value);

		musicEffect = 50;
		musicVolume = 50;
		musicVolumeClosed = false;
		musicEffectClosed = false;

		return true;
	}
	else
		return false;
}
void MenuLayer::settingBtnCallback(CCObject *pSender, TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{
		CCLOG("game setting show");
		
		SettingMenuLayer* gameSetLayer = new SettingMenuLayer();
		if(gameSetLayer && gameSetLayer->init(musicVolumeClosed,musicEffectClosed,musicVolume,musicEffect))
		{
			gameSetLayer->autorelease();
		}
		else
		{
			CC_SAFE_DELETE(gameSetLayer);
		}
	
		GameScene::shareGameScene()->addChild(gameSetLayer,20);
		
	}
}

void MenuLayer::setVoice(bool VolumeClosed,bool EffectClosed,int Volume,int Effect)
{
	musicVolumeClosed = VolumeClosed;
	musicEffectClosed = EffectClosed;
	musicVolume = Volume;
	musicEffect = Effect;
}

void MenuLayer::setScore(const char *value)
{
	scoreLabel->setStringValue(value);

}

const char* MenuLayer::getScore()
{
	return scoreLabel->getStringValue();
}