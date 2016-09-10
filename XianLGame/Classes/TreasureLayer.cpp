#include "TreasureLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalUnits.h"
#include "YZNetExport.h"

using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;

#define Name_Local_Head_Pic			"PersonalHead/local_head_"
#define Name_Custom_Head_Pic		"/sdcard/HeadPic_YL.png"

TreasureLayer* TreasureLayer::createLayer(MoneyChangeNotify* delegate)
{
	TreasureLayer *pRet = new TreasureLayer(); 
	if (pRet && pRet->init()) 
	{ 
		pRet->setChangeDelegate(delegate);
		pRet->autorelease(); 
		return pRet; 
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}

bool TreasureLayer::init()
{
	if ( !Layer::init() )
	{
		return false;
	}
	Size winSize = Director::getInstance()->getWinSize();
	
	_uiTreasureLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("TreasureLayer/TreasureLayer_1.json");
	_uiTreasureLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	_uiTreasureLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(_uiTreasureLayer);

	// -- btn
	Button * btn_tbox1 = (Button*)(Helper::seekWidgetByName(_uiTreasureLayer,"t_box1"));
	btn_tbox1->addTouchEventListener(CC_CALLBACK_2(TreasureLayer::onBtnClicked,this));
	btn_tbox1->setTag(BTN_TBOX1);

	Button * btn_tbox2 = (Button*)(Helper::seekWidgetByName(_uiTreasureLayer,"t_box2"));
	btn_tbox2->addTouchEventListener(CC_CALLBACK_2(TreasureLayer::onBtnClicked,this));
	btn_tbox2->setTag(BTN_TBOX2);

	Button * btn_tbox3 = (Button*)(Helper::seekWidgetByName(_uiTreasureLayer,"t_box3"));
	btn_tbox3->addTouchEventListener(CC_CALLBACK_2(TreasureLayer::onBtnClicked,this));
	btn_tbox3->setTag(BTN_TBOX3);

	Button * btn_tbox4 = (Button*)(Helper::seekWidgetByName(_uiTreasureLayer,"t_box4"));
	btn_tbox4->addTouchEventListener(CC_CALLBACK_2(TreasureLayer::onBtnClicked,this));
	btn_tbox4->setTag(BTN_TBOX4);

	Button * storeBack = (Button*)(Helper::seekWidgetByName(_uiTreasureLayer,"btn_close"));
	storeBack->addTouchEventListener(CC_CALLBACK_2(TreasureLayer::onBtnClicked,this));
	storeBack->setTag(BTN_EXIT);

	// --TODO 游戏宝箱积分
	_score = (Text*)(Helper::seekWidgetByName(_uiTreasureLayer,"label_score"));
	char goldStr[10];
	sprintf(goldStr, "%d", PlatformLogic()->loginResult.i64Money);
	_score->setString(goldStr);

	// -- TODO 头像image
	_headPic = (ImageView*)(Helper::seekWidgetByName(_uiTreasureLayer,"head"));
	MSG_GP_R_LogonResult& LogonResult = PlatformLogic()->loginResult;
	auto picId =  LogonResult.bLogoID;
	char picName[60];
	memset(picName, 0, sizeof(picName));
	sprintf(picName, "%s%02d%s", Name_Local_Head_Pic,picId,".png");
	if (picId == 45)
	{
		sprintf(picName, "%s", Name_Custom_Head_Pic);
	}
	_headPic->loadTexture(picName);

	_roleStar = (ImageView*)(Helper::seekWidgetByName(_uiTreasureLayer,"role_star"));
	
	// -- 宝箱积分进度条
	_progress = (ProgressTimer*)(Helper::seekWidgetByName(_uiTreasureLayer,"progress_on"));

	return true;
}

void TreasureLayer::onImageClicked(Ref * pSender,Widget::TouchEventType type)
{
}

void TreasureLayer::onBtnClicked(Ref * pSender,Widget::TouchEventType type)
{
	Button *btn = (Button*)pSender;
	if(type==Widget::TouchEventType::ENDED && btn)
	{
		YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
		log("tag = %d", btn->getTag());
		switch (btn->getTag())
		{
		case BTN_TBOX1:
			{
				// 如果满足领取宝箱的积分 则领取否则显示宝箱
				if (0)
				{
				}
			}
			break;
		case BTN_TBOX2:
			{
			}
			break;
		case BTN_TBOX3:
			{
			}
			break;
		case BTN_TBOX4:
			{
			}
			break;
		case BTN_EXIT:
			{
				removeFromParentAndCleanup(true);
			}
			break;
		default:
			break;
		}
	}
}


void TreasureLayer::setChangeDelegate(MoneyChangeNotify* delegate)
{
	_delegate = delegate;
}

// refresh gold
void TreasureLayer::refreshGold(int change)
{

}