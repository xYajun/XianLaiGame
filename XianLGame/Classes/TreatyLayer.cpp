#include "TreatyLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalUnits.h"
#include "SimpleAudioEngine.h"
#include "GamePrompt.h"
#include "LoginLayer.h"
#include <regex>
#include "PlatformDefine.h"


using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;

using namespace CocosDenshion;

Layer* TreatyLayer::createLayer()
{
	auto layer = TreatyLayer::create();
	return layer;
}

bool TreatyLayer::init()
{
	if ( !Layer::init() )
	{
		return false;
	}

	Size winSize = Director::getInstance()->getWinSize();
	auto uiTreatyLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("TreatyLayer/treatyLayer.json");
	uiTreatyLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	uiTreatyLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(uiTreatyLayer);

	Button* btn_close = (Button*)(Helper::seekWidgetByName(uiTreatyLayer,"btn_close"));
	btn_close->addTouchEventListener(CC_CALLBACK_2(TreatyLayer::onBtnClicked,this));
	btn_close->setTag(BTN_HELPCLOSE);


	return true;
}

void TreatyLayer::onBtnClicked(Ref * pSender,Widget::TouchEventType type)
{
	Button *btn = (Button*)pSender;
	if(type==Widget::TouchEventType::ENDED && btn)
	{
		switch (btn->getTag())
		{
		case BTN_HELPCLOSE:
			{
				removeFromParentAndCleanup(true);
			}
			break;
		default:
			break;
		}
	}
}





