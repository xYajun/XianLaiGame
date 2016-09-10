#include "StoreLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalUnits.h"
#include "YZNetExport.h"
#include "HallLayer.h"
#include "GamePrompt.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "jni/JniHelper.h"
#include <android/log.h>

#if 1
#define  LOG_TAG    "JniHelper"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#else
#define  LOGD(...)
#endif

#endif

using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;


StoreLayer * StoreLayer::_StoreLayer = nullptr;
StoreLayer*  StoreLayer::getInstance()
{
    if (!_StoreLayer)
    {
        _StoreLayer = new StoreLayer();
    }
    return _StoreLayer;
}

StoreLayer* StoreLayer::createLayer(MoneyChangeNotify* delegate , int propId)
{
	StoreLayer *pRet = new StoreLayer();
    pRet->m_iPropId = propId;
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

bool StoreLayer::init()
{
	if ( !Layer::init() )
	{
		return false;
	}
    _StoreLayer = this;
    
	Size winSize = Director::getInstance()->getWinSize();
	_uiStoreLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("storeLayer/ShopLayer.json");
	_uiStoreLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	_uiStoreLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(_uiStoreLayer);

    Button * btn_rmb5 = (Button*)(Helper::seekWidgetByName(_uiStoreLayer,"btn_item1"));
    btn_rmb5->addTouchEventListener(CC_CALLBACK_2(StoreLayer::onBtnClicked,this));
    btn_rmb5->setTag(STORE_BTN_RMB5);
    
    Button * btn_rmb48 = (Button*)(Helper::seekWidgetByName(_uiStoreLayer,"btn_item2"));
    btn_rmb48->addTouchEventListener(CC_CALLBACK_2(StoreLayer::onBtnClicked,this));
    btn_rmb48->setTag(STORE_BTN_RMB48);
    
	Button * btn_close = (Button*)(Helper::seekWidgetByName(_uiStoreLayer,"btn_close"));
	btn_close->setPressedActionEnabled(true);
	btn_close->addTouchEventListener(CC_CALLBACK_2(StoreLayer::onFcBtnClicked,this));
	btn_close->setTag(STORE_BTN_EXIT);

	return true;
}
void StoreLayer::initpanelType1()
{
	char buffer[128];	
	for (int i=1;i<6;i++)
	{
		/*memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "btn_zType%d",i);
		Button* btn_treasure = (Button*)(Helper::seekWidgetByName(_uiStoreLayer,buffer));
		btn_treasure->addTouchEventListener(CC_CALLBACK_2(StoreLayer::onBtnClicked,this));
		btn_treasure->setTag(STORE_TREASURE_BTN_BUY1+i-1);*/
	}
}
void StoreLayer::initpanelType2()
{
	
}
void StoreLayer::initpanelType3()
{
	
}


void StoreLayer::onBtnClicked(Ref * pSender,Widget::TouchEventType type)
{
	Button *btn = (Button*)pSender;
	if(type==Widget::TouchEventType::ENDED && btn)
	{
		YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
		log("tag = %d", btn->getTag());
        
        if (btn->getTag() == STORE_BTN_RMB5) {
            m_iBuyCardCount =1;
        }else if(btn->getTag() == STORE_BTN_RMB48){
            m_iBuyCardCount =10;
        }
        
        CCLOG("PropId : %d  --  BuyCount : %d " , m_iPropId , m_iBuyCardCount);
        
        int price = 100 * btn->getTag();
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
        JniMethodInfo minfo;
        
        bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/dada/AppActivity","onPay", "(I)V");
        
        if (isHave)
        {
            jint totalPrice = (jint)price;
            minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID , totalPrice);
        }
#endif
    }
}
void StoreLayer::onFcBtnClicked( Ref * pSender,Widget::TouchEventType type )
{
	Button *btn = (Button*)pSender;
	if(type==Widget::TouchEventType::ENDED && btn)
	{
		YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
		switch (btn->getTag())
		{
		case STORE_BTN_EXIT:
			{
				removeFromParentAndCleanup(true);
			}
			break;
		default:
			break;
		}
	}
}


void StoreLayer::openStore(Node* parent, Vec2 vec, int zorder, int tag)
{
	CCAssert(nullptr != parent, "parent is nullptr");

	Size winSize = Director::getInstance()->getWinSize();
	this->setPosition(vec);
	parent->addChild(this, zorder, tag);
	/*parent->runAction(EaseSineOut::create(MoveBy::create(0.2f, Vec2(-winSize.width, 0))));*/
	parent->runAction(MoveBy::create(0.3f, Vec2(-winSize.width, 0)));
}

void StoreLayer::setChangeDelegate(MoneyChangeNotify* delegate)
{
	_delegate = delegate;
}

// refresh gold
void StoreLayer::refreshGold(int change)
{

}


bool StoreLayer::isDigitStr( const char* cstr )
{
	if (NULL == cstr || cstr[0] == 0)  
	{  
		return false;  
	}  

	int len = strlen(cstr);  
	int pos = 0;  
	if (cstr[0] == '-' || cstr[0] == '+')  
	{  
		if (len <= 1)   
		{  
			return false;  
		}  

		pos++;          
	}  

	while (pos < len)  
	{  
		if (cstr[pos] < '0' || cstr[pos] > '9')  
		{  
			return false;  
		}  
		pos++;  
	}  

	return true;  
}

bool StoreLayer::storeEventSelector( YZSocketMessage* socketMessage )
{
	UINT bAssistantID = socketMessage->messageHead.bAssistantID;
	UINT bHandleCode = socketMessage->messageHead.bHandleCode;

	 
	return true;
}


void getPayInfo(std::string errorCode, std::string errorStr)
{
    CCLOG("GetPayInfo -- errorCode : %s  errorStr : %s " , errorCode.c_str(), errorStr.c_str());
  
    if (std::atoi(errorCode.c_str()) == 0) {
        MSG_PROP_C_BUY_ROOMCARD data;
        memset(&data, 0, sizeof(data));
        data.dwUserID = PlatformLogic()->loginResult.dwUserID;
        data.nPropID =StoreLayer::getInstance()->m_iPropId;
        data.nPropCount = StoreLayer::getInstance()->m_iBuyCardCount;
        
        YZRoomLogic::getInstance()->sendData(MDM_GR_PROP, ASS_PROP_ROOMCARD_BUY, &data, sizeof(data));
    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT void JNICALL Java_com_dada_hz_wxapi_WXPayEntryActivity_buyRoomCardCallBack(JNIEnv *env, jobject thiz, jstring _errorCode,jstring _errorStr) {
        const char* errorCode = env->GetStringUTFChars(_errorCode, nullptr);
        const char* errorStr = env->GetStringUTFChars(_errorStr, nullptr);
        getPayInfo(errorCode, errorStr);
    }
}
#endif
