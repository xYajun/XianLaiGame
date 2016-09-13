#include "PlayerInfoLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalUnits.h"
#include "SimpleAudioEngine.h"
#include "GamePrompt.h"
#include "LoginLayer.h"
#include <regex>
#include "PlatformDefine.h"
#include "UI/HttpSprite.h"

#define BTN_INFOCLOSE 10001
#define BTN_LOGINOUT  10002


using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;

using namespace CocosDenshion;

Layer* PlayerInfoLayer::createLayer(PlayerInfo pInfo)
{
    
    PlayerInfoLayer * pRet = new PlayerInfoLayer();
    pRet->uInfo = pInfo;
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
}

char * int2ipstr (const int ip, char *buf)
{
    sprintf (buf, "%u.%u.%u.%u",
             (unsigned char) * ((char *) &ip + 0),
             (unsigned char) * ((char *) &ip + 1),
             (unsigned char) * ((char *) &ip + 2), (unsigned char) * ((char *) &ip + 3));
    return buf;
}

bool PlayerInfoLayer::init()
{
	if ( !Layer::init() )
	{
		return false;
	}

	Size winSize = Director::getInstance()->getWinSize();
	auto uiPlayerInfoLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("roleInfo/RoleInfo.json");
	uiPlayerInfoLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	uiPlayerInfoLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(uiPlayerInfoLayer);
    
    auto _headImg = (ImageView*)(Helper::seekWidgetByName(uiPlayerInfoLayer,"headImg"));
    _headImg->setVisible(false);
    auto httpSp = HttpSprite::create(uInfo.headUrl, "roleInfo/res2/thumb.png");
    httpSp->setPosition(_headImg->getPosition());
    uiPlayerInfoLayer->addChild(httpSp);
    
    ui::Text* label_name = (ui::Text*)(Helper::seekWidgetByName(uiPlayerInfoLayer,"label_name"));
    label_name->setString(GBKToUtf8(uInfo.nickName.c_str()));
    
    auto label_sex = (ImageView*)(Helper::seekWidgetByName(uiPlayerInfoLayer,"label_sex"));
    if (uInfo.bBoy) {
        label_sex->loadTexture("roleInfo/res2/nan.png");
    }else{
        label_sex->loadTexture("roleInfo/res2/nv.png");
    }
    
    ui::Text* label_id = (ui::Text*)(Helper::seekWidgetByName(uiPlayerInfoLayer,"label_id"));
    
    char id[8];
    sprintf(id, "%d", uInfo.dwUserID);
    label_id->setString(id);
    
    ui::Text* label_ip = (ui::Text*)(Helper::seekWidgetByName(uiPlayerInfoLayer,"label_ip"));
    char buf[50];
    char * ip = int2ipstr(uInfo.dwUserIP, buf);
    label_ip->setString(ip);
    
    Button* btn_close = (Button*)(Helper::seekWidgetByName(uiPlayerInfoLayer,"btn_close"));
    btn_close->addTouchEventListener(CC_CALLBACK_2(PlayerInfoLayer::onBtnClicked,this));
    btn_close->setTag(BTN_INFOCLOSE);
    
//    Button* btn_loginOut = (Button*)(Helper::seekWidgetByName(uiPlayerInfoLayer,"btn_outLogin"));
//    btn_loginOut->addTouchEventListener(CC_CALLBACK_2(PlayerInfoLayer::onBtnClicked,this));
//    btn_loginOut->setTag(BTN_LOGINOUT);
//    btn_loginOut->setVisible(false);
//    if (uInfo.isHall) {
//        btn_loginOut->setVisible(true);
//    }

	return true;
}

void PlayerInfoLayer::onBtnClicked(Ref * pSender,Widget::TouchEventType type)
{
	Button *btn = (Button*)pSender;
	if(type==Widget::TouchEventType::ENDED && btn)
	{
		switch (btn->getTag())
		{
		case BTN_INFOCLOSE:
			{
				removeFromParentAndCleanup(true);
			}
			break;
                
            case BTN_LOGINOUT:
            {
                RoomLogic()->closeRoom();
                PlatformLogic()->closePlatform();
                auto scene =LoginLayer::createScene();
                Director::getInstance()->replaceScene(scene);
            }
                break;
            default:
			break;
		}
	}
}





