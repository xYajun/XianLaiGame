#include "HelpLayer.h"
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

Layer* HelpLayer::createLayer()
{
	auto layer = HelpLayer::create();
	return layer;
}

bool HelpLayer::init()
{
	if ( !Layer::init() )
	{
		return false;
	}

	Size winSize = Director::getInstance()->getWinSize();
	auto uiHelpLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("HelpLayer/How2Play.json");
	uiHelpLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	uiHelpLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(uiHelpLayer);

	Button* btn_close = (Button*)(Helper::seekWidgetByName(uiHelpLayer,"btn_close"));
	btn_close->addTouchEventListener(CC_CALLBACK_2(HelpLayer::onBtnClicked,this));
	btn_close->setTag(BTN_HELPCLOSE);


	return true;
}

void HelpLayer::onBtnClicked(Ref * pSender,Widget::TouchEventType type)
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





