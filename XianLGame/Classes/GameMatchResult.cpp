#include "GameMatchResult.h"
#include "HallLayer.h"
#include "FontConfig.h"
#include "StoreLayer.h"
#include "PlatformDefine.h"
#include "GamePrompt.h"
#include "PlatformConfig.h"
#include "LoginLayer.h"
#include "GameQueueWait.h"
#include "HallLayer.h"


Layer* GameMatchResult::createMatchRet(int rank)
{
    return GameMatchResult::create(rank);
}

GameMatchResult::GameMatchResult(int rank) 
{
	_rank = rank;
}

GameMatchResult* GameMatchResult::create(int rank) 
{
	GameMatchResult *pRet = new GameMatchResult(rank); 
	if (pRet && pRet->init()) 
	{ 
		pRet->autorelease(); 
		return pRet; 
	} 
	else 
	{ 
		delete pRet; 
		pRet = NULL; 
		return NULL; 
	} 
}

GameMatchResult::~GameMatchResult()
{

}

bool GameMatchResult::init()
{
	if (!YZLayer::init()) return false;

	enableKeyboard();

	auto tableWidget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("matchresult/matchresult_1.json");
	addChild(tableWidget);

	auto rank = dynamic_cast<ui::TextAtlas*>(ui::Helper::seekWidgetByName(tableWidget, "AtlasLabel_rank"));
	char str_rank[20];
	sprintf(str_rank, "%d", _rank);
	rank->setString(str_rank);

	auto Button_check = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(tableWidget, "Button_check"));
	Button_check->addTouchEventListener(CC_CALLBACK_2(GameMatchResult::menuClickedCallback, this));
}


void GameMatchResult::menuClickedCallback(Ref* pRef, Widget::TouchEventType touchtype)
{
	if (Widget::TouchEventType::ENDED != touchtype)	return;

	HallLayer::returnHallLayer();
}



void GameMatchResult::onExit()
{
	YZLayer::onExit();
}

