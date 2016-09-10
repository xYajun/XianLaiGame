#include "LoginLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalUnits.h"
#include "HallLayer.h"
#include "FontConfig.h"
#include "GamePrompt.h"
#include "YZCommon/YZConverCode.h"
#include "Util/JzLog.h"
#include "TreatyLayer.h"
using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;

#define  Email_Example_Str		"例如:xxxxxx@xx.com"

#define Name_Local_Head_Pic			"PersonalHead/local_head_"
#define Name_sex_PicOn			"PersonalHead/kuang_on.png"
#define Top_Pic					"LoginLayer/common/wuyan.png"

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

LoginLayer * LoginLayer::_GameLoginLayer = nullptr;

Scene* LoginLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = LoginLayer::create();
    JzLog::shareLog()->bindViewToScene(scene);
    scene->addChild(layer);
    return scene;
}

bool LoginLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    g_GlobalUnits.serverip = "211.149.160.248";
    //g_GlobalUnits.serverip = "139.129.211.170";
    //g_GlobalUnits.serverip = "112.74.17.121";
    //g_GlobalUnits.serverip = "192.168.199.250";
    
    JzDebug("服务器IP ： %s " , g_GlobalUnits.serverip.c_str());
    
    g_GlobalUnits.isOpenRoom = true;
    
    enableKeyboard();
    SaveBooleanToXML("loginType",false);
    m_count = 0;
    memset(&_loginByName, 0, sizeof(MSG_GP_S_LogonByNameStruct));
    _isLogin = false;
    _isQuickLogin = false;
    _isRegister = false;
    _isChechName = false;
    _isKickNickImg = false;
    _isLoginSucess=false;
    _isNeedLogin=false;
    selectImg = nullptr;
    memset(_szPass, 0, 32);
    memset(_regName, 0, 32);
    memset(_regPswd, 0, 32);
    _regSex = 0;
    _headImgId = 1;
    
    std::string save_pswd = LoadStringFromXML(SAVE_PASSWORD);
    
    if (save_pswd.length() == 0)
    {
        memcpy(_szPass, DEFAULT_PASSWORD, sizeof(DEFAULT_PASSWORD));
    }
    else
    {
        memcpy(_szPass, save_pswd.c_str(), save_pswd.length());
    }
    creatInitLoginLayer();
    
    YZAudioEngine::getInstance()->playBackgroundMusic(GAME_BACKGROUND_MUSICYL, true);
    
    return true;
}

void LoginLayer::update(float dt){
    times -= dt;
    if (PlatformLogic()->connected()) {
        LoadingLayer::removeLoading(this);
        this->unscheduleUpdate();
    }else{
        if (times <= 0) {
            times = 1.5f;
            PlatformLogic()->connectPlatform();
        }
    }
}

LoginLayer*  LoginLayer::getInstance()
{
    if (!_GameLoginLayer)
    {
        _GameLoginLayer = new LoginLayer();
    }
    return _GameLoginLayer;
}

void  LoginLayer::creatInitLoginLayer()
{
    Size winSize = Director::getInstance()->getWinSize();
    
    _uiInitLoginLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("LoginLayer/LoginLayer_1.json");
    _uiInitLoginLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    _uiInitLoginLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    _uiInitLoginLayer->setTag(LOGIN_LAYER);
    addChild(_uiInitLoginLayer);
    
    ImageView* login_bg = (ImageView*)(Helper::seekWidgetByName(_uiInitLoginLayer,"Login_bg"));
    float _xScale = winSize.width / login_bg->getContentSize().width;
    float _yScale = winSize.height/ login_bg->getContentSize().height;
    login_bg->setScaleX(_xScale);
    login_bg->setScaleY(_yScale);
    
    agreeCB = (CheckBox*)(Helper::seekWidgetByName(_uiInitLoginLayer,"cb_agree"));
    agreeCB->setSelected(true);
    
    
    Button* wxButton = (Button*)(Helper::seekWidgetByName(_uiInitLoginLayer,"btn_weixinLogin"));
    wxButton->addTouchEventListener(CC_CALLBACK_2(LoginLayer::onBtnClicked,this));
    wxButton->setTag(WXLOGIN_BTN);
    
    Button* xyButton = (Button*)(Helper::seekWidgetByName(_uiInitLoginLayer,"btn_lookXY"));
    xyButton->addTouchEventListener(CC_CALLBACK_2(LoginLayer::onBtnClicked,this));
    xyButton->setTag(LOOK_XY_BTN);
    
    PlatformLogic()->connectPlatform();
    
    LoadingLayer::createLoading(this, gFontConfig_22, Word_Loading, LOADING);
    
    this->scheduleUpdate();

    times = 1.5f;
    
    
    
    
    
    
    
    
    
}

void LoginLayer::onBtnClicked(Ref * pSender,Widget::TouchEventType type)
{
    Button *btn = (Button*)pSender;
    if(type==Widget::TouchEventType::ENDED && btn)
    {
        YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
        
        PlatformLogic()->connectPlatform();

        CallFuncN *ccFunc = CallFuncN::create(CC_CALLBACK_1(LoginLayer::buttonActionEnd,this,btn->getTag()));
        btn->runAction(Sequence::create(ScaleTo::create(0.2f,1.2f),
                                        ScaleTo::create(0.1f,1.0f),ccFunc,nullptr));
    }
}


//微信登录
void LoginLayer::loginPlatform()
{
    if (PlatformLogic()->connected())
    {
        std::string  nicknameStr = LoadStringFromXML("nickname");
        std::string unionidStr = LoadStringFromXML("unionid");
        std::string headUrl = LoadStringFromXML("headPicURL");
        JzDebug("上传头像连接到服务器... 链接 : %s " , headUrl.c_str());
        CCLOG("nickname : %s " , nicknameStr.c_str());
        platformLogin(nicknameStr.c_str(), MD5_CTX::MD5String(DEFAULT_PASSWORD), 0,unionidStr,headUrl);
    }
    else
    {
        PlatformLogic()->connectPlatform();
    }
}


//游客登录
void LoginLayer::guestLogin()
{
    // uid存在,直接登录;
    if (PlatformLogic()->connected()) {
        int uid = LoadIntegerFromXML(SAVE_USERID);
        CCLOG("uid : %d" , uid);
        if (uid > 0)
        {
            LoadingLayer::createLoading(this, gFontConfig_22, "正在登陆...", LOADING);

            std::string  nicknameStr = LoadStringFromXML("nickname");
            std::string unionidStr = LoadStringFromXML("unionid");
            std::string headUrl = LoadStringFromXML("headPicURL");
            JzDebug("上传头像连接到服务器... 链接 : %s " , headUrl.c_str());
            CCLOG("nickname : %s " , nicknameStr.c_str());
            platformLogin(nicknameStr.c_str(), MD5_CTX::MD5String(DEFAULT_PASSWORD), 0,unionidStr,headUrl);

            //uid登陆
            //PlatformLogic()->platformLogin(DEFAULT_USERNAME, MD5_CTX::MD5String(_szPass), uid);
        }
        else
        {
            #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            
            JniMethodInfo minfo;
            
            bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/dada/AppActivity","loginWeixin", "()V");
            
            if (isHave)
            {
                minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
            }
            #endif

        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            
            LoadingLayer::createLoading(this, gFontConfig_22, Word_Loading, LOADING);
            
            // uid存在,直接登录;
            
            if (PlatformLogic()->connected()) {
                int uid = LoadIntegerFromXML(SAVE_USERID);
                if (uid > 0)
                {
                    //uid登陆
                    PlatformLogic()->platformLogin(DEFAULT_USERNAME, MD5_CTX::MD5String(_szPass), uid);
                }
                else
                {
                    //游客注册
                    PlatformLogic()->platformRegister(true, 0, DEFAULT_USERNAME, MD5_CTX::MD5String(DEFAULT_PASSWORD), Utf8ToGB("游客"), _regSex,"",_headImgId);
                }
            }else{
                LoadingLayer::removeLoading(this);
                
                PlatformLogic()->connectPlatform();
            }
            
        #endif
 
        }
    }else{
        LoadingLayer::removeLoading(this);
        
        PlatformLogic()->connectPlatform();
    }
}

void LoginLayer::userRegister()
{
    if (PlatformLogic()->connected())
    {
        
        PlatformLogic()->platformRegister(false, 0, _regName , MD5_CTX::MD5String(_regPswd), "", _regSex,_regPswd,_headImgId);
    }
    else
    {
        PlatformLogic()->connectPlatform();
    }
    
}

void LoginLayer::checkName()
{
    if (PlatformLogic()->connected())
    {
        platformCheckLogin(_regName);
    }
    else
    {
        PlatformLogic()->connectPlatform();
    }
}


void LoginLayer::onHandleDisConnect()
{
    LoadingLayer::removeLoading(this);
}

void LoginLayer::onHandleConnectMessage(bool result)
{
    if (result)
    {
        YZLOG_DEBUG("connect success");
        if (_isQuickLogin)
        {
            guestLogin();
        }
        else if(_isLogin)
        {
            // loginPlatform();
        }
        else if (_isRegister)
        {
            userRegister();
        }
        else if (_isChechName)
        {
            checkName();
        }
    }
    else
    {
        LoadingLayer::removeLoading(this);
        YZLOG_DEBUG("connect fail");
    }
}

void LoginLayer::onHandleLoginMessage(bool result, UINT dwErrorCode)
{
    /*  if (_isNeedLogin)
     {
     _isNeedLogin=false;
     PlatformLogic()->closePlatform();
     return;
     }
     _isLoginSucess=true;*/
    LoadingLayer::removeLoading(this);
    if (result)
    {
        YZLOG_DEBUG("login success!");
        YZLOG_DEBUG("进入大厅...");
        HallLayer::createScene();
    }
    else
    {
        std::string errorText("");
        if (ERR_GP_USER_NO_FIND == dwErrorCode)
        {
            errorText = Word_Wrong_Name;
        }
        else if (ERR_GP_USER_PASS_ERROR == dwErrorCode)
        {
            errorText = Word_Wrong_Pass;
        }
        else if (ERR_GP_USER_LOGON == dwErrorCode)
        {
            errorText = Word_Logined;
        }
        else if (DTR_GP_PLACE_VER_LOW_LIMIT	 == dwErrorCode)
        {
            errorText = Word_Down;
        }
        else if (DTR_GP_PLACE_VER_MAX_LIMIT == dwErrorCode)
        {
            errorText = Word_Up;
        }
        else
        {
            errorText = Word_Down;
        }
        GamePromptLayer::create()->showPrompt(errorText.c_str());
    }
}

void LoginLayer::onHandleUserRegisterMessage(void* registerStruct, UINT bAssistantID)
{
    LoadingLayer::removeLoading(this);
    switch (bAssistantID)
    {
        case ASS_GP_VISITOR_REGISTER_RESULT:
        {
            MSG_GP_R_VisitorRegister* visitorRegister  = (MSG_GP_R_VisitorRegister *)registerStruct;
            if (visitorRegister->dwUserID > 0)
            {
                //保存UID到本地XML
                SaveBooleanToXML(SAVE_ISGUEST, true);
                SaveIntegerToXML(SAVE_USERID, visitorRegister->dwUserID);
                guestLogin();
            }
            else
            {
                std::string errorText = Word_Wrong_GUEST_REG;
                GamePromptLayer::create()->showPrompt(errorText.c_str());
            }
        }
            break;
        case ASS_GP_VISITOR_BIND_RESULT:
        {
            
        }
            break;
        case ASS_GP_USER_REGISTER_RESULT:
        {
            /*
             MSG_GP_R_UserRegister* userRegister  = (MSG_GP_R_UserRegister *)registerStruct;
             if (userRegister->dwUserID > 0)
             {
             SaveBooleanToXML(SAVE_ISGUEST, false);
             //保存UID到本地XML
             SaveIntegerToXML(SAVE_USERID, userRegister->dwUserID);
             
             std::string nickname=LoadStringFromXML("nickname");
             std::string unionidStr = LoadStringFromXML("unionid");
             auto prompt = GamePromptLayer::create();
             prompt->showPrompt(Word_Success_USER_REG);
             prompt->setCallBack([&]()
             {
             // 鍙戦�璐﹀彿瀵嗙爜鐧婚檰
             LoadingLayer::createLoading(this, gFontConfig_22, Word_Loading, LOADING);
             
             platformLogin(nickname,MD5_CTX::MD5String(DEFAULT_PASSWORD), 0,unionidStr);
             
             });
             }
             else
             {
             std::string errorText = Word_Wrong_USER_REG;
             GamePromptLayer::create()->showPrompt(errorText.c_str());
             }
             */
        }
            break;
        case ASS_GP_USER_NAME_VERIFICTAION_RESULT:
        {
            MSG_GP_R_User_Verification* userRegister  = (MSG_GP_R_User_Verification *)registerStruct;
            if (userRegister->dwRes == 0)
            {
                
            }
            else
            {
                GamePromptLayer::create()->showPrompt(Word_Wrong_GUEST_BIND);
            }
        }
            break;
        default:
            break;
    }
}

void LoginLayer::returnLogin()
{
    auto scene = Scene::create();
    auto mainlayer = LoginLayer::create();
    scene->addChild(mainlayer);
    Director::getInstance()->replaceScene(TransitionSlideInL::create(0.3f, scene));
}

bool LoginLayer::isDigitStr(const char* cstr)
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


void LoginLayer::buttonActionEnd( Node* sender,int data )
{
    switch (data)
    {
        case WXLOGIN_BTN:
        {
            if (agreeCB->isSelected()) {

                if (PlatformLogic()->connected())
                {
                    _isLogin = true;
                    guestLogin();
                }
                else
                {
                    PlatformLogic()->connectPlatform();
                }
                
            }else{
                CHAR buf[512];
                sprintf(buf, ("请先阅读并且同意游戏协议!"));
                YZLOG_ERROR("%s", GBKToUtf8(buf));
                GamePromptLayer::create()->showPrompt(GBKToUtf8(buf));
            }
            
        }
            break;
        case LOOK_XY_BTN:
        {
            auto layer = TreatyLayer::create();
            addChild(layer, 999);
        }
            break;
        default:
            break;
    }
}

void getWxinfo(std::string headUrl, std::string unionid, std::string nickname, std::string sex)
{
	 struct tm* tm;
	    long total_second = 0;
	    time_t timep;
	    time(&timep);
	    tm = localtime(&timep);
	    total_second = timep;
	    char str[60];
	    sprintf(str,"%d",total_second);
	    std::string uid=LoadStringFromXML("unionid", unionid);
	    SaveStringToXML("headPicURL", headUrl);
	    SaveBooleanToXML("loginType",true);
	    SaveStringToXML("unionid", unionid);
	    SaveStringToXML("nickname", nickname);

	    //if(strcmp(unionid.c_str(),uid.c_str())==0)
	    //{
	    //LoginLayer::getInstance()->loginPlatform();
	    //return;
	    //}

	    log("loginView::getWxinfo headPic=%s ", headUrl.c_str());
	    log("loginView::getWxinfo nickname=%s ", nickname.c_str());
	    log("loginView::getWxinfo unionid=%s ", unionid.c_str());

	    SaveStringToXML("sex", sex);

	    LoginLayer::getInstance()->loginPlatform();
}

//鎵嬫満杩斿洖閿洃鍚洖璋冨嚱鏁�
void LoginLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (EventKeyboard::KeyCode::KEY_BACK == keyCode)
    {
        exit(-1);
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    if (EventKeyboard::KeyCode::KEY_BACKSPACE == keyCode)
    {
        exit(-1);
    }
#endif
    
}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    
    JNIEXPORT void JNICALL Java_com_dada_hz_wxapi_WXEntryActivity_nativeInfoWxCode(JNIEnv *env, jobject thiz, jstring headUrl,jstring unionid,jstring nickname,jstring sex) {
        
        CCLOG("Java_com_pdk_dada_wxapi_nativeInfoWxCode");
        
        const char* pHeadUrl = env->GetStringUTFChars(headUrl, nullptr);
        const char* pUnionid = env->GetStringUTFChars(unionid, nullptr);
        const char* pNickname = env->GetStringUTFChars(nickname, nullptr);
        const char* pSex = env->GetStringUTFChars(sex, nullptr);
        

        getWxinfo(pHeadUrl, pUnionid, pNickname, pSex);
    }
}
#endif
