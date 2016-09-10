#include "FishKingGameUserMessageBox.h"
#include <string>
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace ui;

namespace FishKing
{
	static const INT USERDATA_BOX			= 10;    //玩家资料框
	static const char*  CSB_FILE			= "fishKing/userinfo/UserNode.csb";
	static const char* MEN_HEAD				= "fishKing/userinfo/men_head.png";
	static const char* WOMEN_HEAD			= "fishKing/userinfo/women_head.png";


	FishKingUserMessageBox* FishKingUserMessageBox::createMessageBox(cocos2d::Node* parent, INT userID, LLONG userMoney)
	{
		auto pRet = new FishKingUserMessageBox();
		if (pRet && pRet->init(parent, userID, userMoney))
		{
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}

	bool FishKingUserMessageBox::init(cocos2d::Node* parent, INT userID, LLONG userMoney)
	{
		if ( !YZDialogBase::init(parent)) return false;

		Size winSize = Director::getInstance()->getWinSize();

		
		_loader = dynamic_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("fishKing/userinfo/UserUiRes.json"));
		//_loader = CSLoader::createNode(CSB_FILE);
		_loader->setPosition(Vec2(winSize / 2));
		addChild(_loader);
		////创建一个点击事件
		//auto MyListener = EventListenerTouchOneByOne::create();
		//MyListener->setSwallowTouches(true);//阻止触摸向下传递
		//MyListener->onTouchBegan = [=](Touch* touch, Event* event)
		//{
		//	auto bg = (ImageView*)( ui::Helper::seekWidgetByName(_loader,"Image_bg"));

		//	Rect rect = Rect(0, 0, bg->getContentSize().width, bg->getContentSize().height);        
		////	auto pos = bg->convertToNodeSpace(touch->getLocation());
		//	if (!rect.containsPoint(touch->getLocation()))
		//	{
		//		hide();
		//	}
		//	return true;
		//};
		//_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, this);

		
		auto bg = (ImageView*)( ui::Helper::seekWidgetByName(_loader,"Image_bg"));

		auto listener = EventListenerTouchOneByOne::create();
	

		listener->onTouchBegan = [winSize, bg,this](Touch* t, Event* e){
			if ((bg->getBoundingBox().containsPoint(t->getLocation())))
			{
				return true;
			}
			else
			{
				hide();
			}
			return false;
		};

		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, bg);

		//玩家信息结构体
		UserInfoStruct* LogonResult = UserInfoModule()->findUser(userID);
		if (nullptr == LogonResult)
		{
			return true;
		}
		char str[32];
		auto pic = (ImageView*) ( ui::Helper::seekWidgetByName(_loader,"Image_userFrame"));
		auto pic_size = pic->getContentSize();
		

		//玩家头像
		char name[60];
		memset(name, 0, sizeof(name));
		/*sprintf(name, "platform/PersonalHead/local_head_%02d%s", LogonResult->bLogoID, ".png");*/
		if (LogonResult->bLogoID >= 45)
		{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//			{
//				sprintf(name, "%s%", "/sdcard/HeadPic_YL_.png");
//				if (!FileUtils::getInstance()->isFileExist(name))
//				{
//					sprintf(name, "platform/PersonalHead/local_head_%02d%s", (LogonResult->bLogoID) % 256, ".png");
//				}
//			}
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//			sprintf(name, "%s", "icon2.png");
//#endif
		}
		if (LogonResult->bBoy)
		{
			sprintf(name, "%s", MEN_HEAD);
		}
		else
		{
			sprintf(name, "%s", WOMEN_HEAD);
		}
		auto userIcon = Sprite::create(name);
		userIcon->setAnchorPoint(Vec2(0.5f , 0.5f));
		auto userWidth = userIcon->getContentSize().width;
		auto userHeight = userIcon->getContentSize().height;
		userIcon->setScale(pic->getContentSize().width / userWidth, pic->getContentSize().height / userHeight);
		userIcon->setPosition(Vec2(pic_size.width / 2, pic_size.height / 2));
		pic->addChild(userIcon, -1);

		//玩家昵称
		sprintf(str, "%s", LogonResult->nickName);
		auto name_text = (Text* ) (ui::Helper::seekWidgetByName(_loader,"Text_name"));
		name_text->setString(GBKToUtf8(str));

		//玩家金币
		sprintf(str, "%lld", userMoney);
		auto money_alas = (Text*) ui::Helper::seekWidgetByName(_loader,"AtlasLabel_Coin");
		money_alas->setString(str);

		//玩家经验
		sprintf(str, "%d", LogonResult->dwExperience);
		auto uEXP = (Text*) ui::Helper::seekWidgetByName(_loader,"Text_exp");
		uEXP->setString(str);

		//玩家胜局
		sprintf(str, "%d", LogonResult->uWinCount);
		auto uWinNum = (Text*) ui::Helper::seekWidgetByName(_loader,"Text_shengju");
		uWinNum->setString(str);
		
		//玩家负局
		sprintf(str, "%d", LogonResult->uLostCount);
		auto uLostNum = (Text*) ui::Helper::seekWidgetByName(_loader,"Text_fuju");
		uLostNum->setString(str);
		
		//玩家和局
		sprintf(str, "%d", LogonResult->uMidCount);
		auto uMidNum = (Text*) ui::Helper::seekWidgetByName(_loader,"Text_heju");
		uMidNum->setString(str);

		return true;
	}
}
