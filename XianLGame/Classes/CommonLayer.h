#ifndef CommonLayer_h__
#define CommonLayer_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"


#include "network/HttpClient.h"
#include "YZNetExport.h"


USING_NS_CC;

using namespace ui;
using namespace cocostudio;
using namespace network;


#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif


// btn tag

#define PW_CONFIRM	1007
#define PW_CANCEL	1008

#define MAIL_CONFIRM	1010
#define MAIL_CANCEL		1011

#define PHONE_CONFIRM	1020
#define PHONE_CANCEL	1021

#define TRUMPET_CONFIRM	1030
#define TRUMPET_CANCEL	1031

// image tag



// ******************** 设置密码 ***************************
class SetPasswordLayer : public Layer
{
    
private:
    Widget* _uiPassWordLayer;
    TextField* _oldpw;
    TextField* _newpw;
    TextField* _confirmpw;
    
    std::string _userPswd;
    std::string _userMD5Pswd;
    
public:
    static cocos2d::Layer* createLayer();
    virtual bool init();
    
    void onBtnClicked(Ref * pSender,Widget::TouchEventType type);
    bool modifyPasswordSelector(YZSocketMessage* socketMessage);
    
    CREATE_FUNC(SetPasswordLayer);
    
};


// ******************** 绑定邮箱 ***************************
class SetEmailLayer : public Layer
{
    
private:
    Widget* _uiEmailLayer;
    
    TextField* _userEmail;
    
public:
    static cocos2d::Layer* createLayer();
    virtual bool init();
    
    void onBtnClicked(Ref * pSender,Widget::TouchEventType type);
    
    bool verifyEmailAddress(const std::string& email);
    
    CREATE_FUNC(SetEmailLayer);
    
};


// ******************** 绑定手机 ***************************
class SetPhoneLayer : public Layer
{
    
private:
    Widget* _uiPhoneLayer;
    TextField* _userPhone;
    
public:
    static cocos2d::Layer* createLayer();
    virtual bool init();
    
    void onBtnClicked(Ref * pSender,Widget::TouchEventType type);
    
    CREATE_FUNC(SetPhoneLayer);
    
};



// ******************** 喇叭发送世界消息 ***************************
class TrumpetLayer : public Layer
{
    
private:
    Widget* _uiTrumpetLayer;
    TextField* _userMsg;
    
public:
    static cocos2d::Layer* createLayer();
    virtual bool init();
    
    void onBtnClicked(Ref * pSender,Widget::TouchEventType type);
    
    CREATE_FUNC(TrumpetLayer);
    
};

#endif // CommonLayer_h__

