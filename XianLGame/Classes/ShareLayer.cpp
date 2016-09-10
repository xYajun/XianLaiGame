#include "ShareLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalUnits.h"
#include "SimpleAudioEngine.h"
#include "GamePrompt.h"
#include "LoginLayer.h"
#include <regex>
#include "PlatformDefine.h"
#include "HallLayer.h"
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

using namespace CocosDenshion;
ShareLayer*ShareLayer::_GameShareLayer = nullptr;

Layer* ShareLayer::createLayer()
{
    auto layer = ShareLayer::create();
    return layer;
}

bool ShareLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

ShareLayer*  ShareLayer::getInstance()
{
    if (!_GameShareLayer)
    {
        _GameShareLayer = new ShareLayer();
    }
    return _GameShareLayer;
}

void ShareLayer::shareShot()
{
    utils::captureScreen(CC_CALLBACK_2(ShareLayer::screenShotCallback, this), "/mnt/sdcard/share.png");
}

void ShareLayer::screenShotCallback(bool b, const std::string& name){
    //截屏成功， 调用微信分享
    CCLOG("success %s, file name:%s", b?"true":"false", name.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    JniMethodInfo minfo;
    
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/zhmg/AppActivity","shareImg", "()V");
    
    if (isHave)
    {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
#endif
    
}

