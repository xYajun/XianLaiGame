#include "SocketErrorLayer.h"
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

Layer* SocketErrorLayer::createLayer()
{
	auto layer = SocketErrorLayer::create();
	return layer;
}

bool SocketErrorLayer::init()
{
	if ( !Layer::init() )
	{
		return false;
	}
    
    YZAudioEngine::getInstance()->stopBackgroundMusic();

	Size winSize = Director::getInstance()->getWinSize();
	auto uiSocketErrorLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("erMaJiang/SocketConnectLayer.json");
	uiSocketErrorLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	uiSocketErrorLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(uiSocketErrorLayer);
    
    ImageView* loginSp = (ImageView*)(Helper::seekWidgetByName(uiSocketErrorLayer,"loadingImg"));
    loginSp->setVisible(false);
    Sprite* sp = Sprite::create("erMaJiang/loading/MjLoading1.png");
    sp->setPosition(loginSp->getPosition());
    uiSocketErrorLayer->addChild(sp , 1);
    
    Animation* pAnimation = Animation::create();
    for (int i = 1; i <= 8; i++)
    {
        char fileName[60];
        sprintf(fileName, "erMaJiang/loading/MjLoading%d.png", i);
        pAnimation->addSpriteFrameWithFile(fileName);
    }
    pAnimation->setDelayPerUnit(2.0f / 8);
    pAnimation->setRestoreOriginalFrame(true);
    Animate* pAnimate = Animate::create(pAnimation);
    sp->runAction(RepeatForever::create(pAnimate));
    
    Button* btn_close = (Button*)(Helper::seekWidgetByName(uiSocketErrorLayer,"reLogin"));
	btn_close->addTouchEventListener(CC_CALLBACK_2(SocketErrorLayer::onBtnClicked,this));
	btn_close->setTag(BTN_HELPCLOSE);

	return true;
}

void SocketErrorLayer::onBtnClicked(Ref * pSender,Widget::TouchEventType type)
{
	Button *btn = (Button*)pSender;
	if(type==Widget::TouchEventType::ENDED && btn)
	{
		switch (btn->getTag())
		{
		case BTN_HELPCLOSE:
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





