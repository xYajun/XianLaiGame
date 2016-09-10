#include "SetLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalUnits.h"
#include "SimpleAudioEngine.h"
#include "GamePrompt.h"
#include "LoginLayer.h"
#include <regex>
#include "PlatformDefine.h"
#include "CommonLayer.h"
#include "HelpLayer.h"


using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;

using namespace CocosDenshion;

Layer* SetLayer::createLayer()
{
	auto layer = SetLayer::create();
	return layer;
}

bool SetLayer::init()
{
	if ( !Layer::init() )
	{
		return false;
	}


	Size winSize = Director::getInstance()->getWinSize();
	auto _uiSetLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("SetLayer/SetLayer.json");
	_uiSetLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	_uiSetLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(_uiSetLayer);

    
    
    auto setBg = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_uiSetLayer, "Image_bg"));
    
    
    auto listener = EventListenerTouchOneByOne::create();
    
    
    listener->onTouchBegan = [winSize, setBg, _uiSetLayer](Touch* t, Event* e){
        if ((setBg->getBoundingBox().containsPoint(t->getLocation())))
        {
            return true;
        }
        else
        {
            _uiSetLayer->removeFromParentAndCleanup(true);
        }
        return false;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, setBg);
    
    // -- btn
   
    
    _btnMusicON= (Button*)(Helper::seekWidgetByName(_uiSetLayer,"btn_musicON"));
    _btnMusicON->setVisible(true);
    _btnMusicON->addTouchEventListener(CC_CALLBACK_2(SetLayer::onBtnClicked,this));
    _btnMusicON->setTag(SET_BTN_MUSICON);
    _btnMusicOFF = (Button*)(Helper::seekWidgetByName(_uiSetLayer,"btn_musicOFF"));
    _btnMusicOFF->setVisible(false);
    _btnMusicOFF->addTouchEventListener(CC_CALLBACK_2(SetLayer::onBtnClicked,this));
    _btnMusicOFF->setTag(SET_BTN_MUSICOFF);

    _btnEffectON= (Button*)(Helper::seekWidgetByName(_uiSetLayer,"btn_effectON"));
    _btnEffectON->setVisible(true);
    _btnEffectON->addTouchEventListener(CC_CALLBACK_2(SetLayer::onBtnClicked,this));
    _btnEffectON->setTag(SET_BTN_EFFECTON);
    _btnEffectOFF = (Button*)(Helper::seekWidgetByName(_uiSetLayer,"btn_effectOFF"));
    _btnEffectOFF->setVisible(false);
    _btnEffectOFF->addTouchEventListener(CC_CALLBACK_2(SetLayer::onBtnClicked,this));
    _btnEffectOFF->setTag(SET_BTN_EFFECOFF);
    
    _btnShakeON = (Button*)(Helper::seekWidgetByName(_uiSetLayer,"btn_shockON"));
    _btnShakeON->setVisible(true);
    _btnShakeON->addTouchEventListener(CC_CALLBACK_2(SetLayer::onBtnClicked,this));
    _btnShakeON->setTag(SET_BTN_SHAKEON);
    _btnShakeOFF = (Button*)(Helper::seekWidgetByName(_uiSetLayer,"btn_shockOFF"));
    _btnShakeOFF->setVisible(false);
    _btnShakeOFF->addTouchEventListener(CC_CALLBACK_2(SetLayer::onBtnClicked,this));
    _btnShakeOFF->setTag(SET_BTN_SHAKEOFF);

	

	return true;
}

void SetLayer::onImageClicked(Ref * pSender,Widget::TouchEventType type)
{
}

void SetLayer::onBtnClicked(Ref * pSender,Widget::TouchEventType type)
{
	Button *btn = (Button*)pSender;
	if(type==Widget::TouchEventType::ENDED && btn)
	{
		YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
		log("tag = %d", btn->getTag());
		switch (btn->getTag())
		{
		case SET_BTN_MUSICON:
			{
                _btnMusicON->setVisible(false);
                _btnMusicOFF->setVisible(true);
                
                SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
			}
			break;
        case SET_BTN_MUSICOFF:
            {
                _btnMusicON->setVisible(true);
                _btnMusicOFF->setVisible(false);
                
                SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
            }
            break;
        case SET_BTN_EFFECTON:
            {
                _btnEffectON->setVisible(false);
                _btnEffectOFF->setVisible(true);
                
                SimpleAudioEngine::getInstance()->setEffectsVolume(0);
            }
            break;
        case SET_BTN_EFFECOFF:
            {
                _btnEffectON->setVisible(true);
                _btnEffectOFF->setVisible(false);
                
                SimpleAudioEngine::getInstance()->setEffectsVolume(1);
            }
            break;
        case SET_BTN_SHAKEON:
            {
                _btnShakeON->setVisible(false);
                _btnShakeOFF->setVisible(true);
            }
            break;
        case SET_BTN_SHAKEOFF:
            {
                _btnShakeON->setVisible(true);
                _btnShakeOFF->setVisible(false);
            }
            break;
            default:
			break;
		}
	}
}

void SetLayer::sliderEvent(Ref *pSender, Slider::EventType type)
{
	switch (type)
	{
	case Slider::EventType::ON_PERCENTAGE_CHANGED:
		{
			Slider* slider = static_cast<Slider*>(pSender);
			switch (slider->getTag())
			{
			case SET_SOUND:
				{
					// set sound vol 
					auto svol = slider->getPercent()/100.0;
					SaveFloatToXML("_SOUND_VOL", svol);
					SaveXmlFlush();
					SimpleAudioEngine::getInstance()->setEffectsVolume(svol);	
				}
				break;
			case SET_MUSIC:
				{
					// set music vol 
					auto mvol  = slider->getPercent()/100.0;
					SaveFloatToXML("_MUSIC_VOL", mvol);
					SaveXmlFlush();
					SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(mvol);
				}
				break;
			default:
				break;
			}
		}
		break;

	default:
		break;
	}
}



// --mail
void SetLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{

}

void SetLayer::editBoxReturn(EditBox* editBox)
{
}

