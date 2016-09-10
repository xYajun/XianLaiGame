#include "CommonLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalUnits.h"
#include "SimpleAudioEngine.h"
#include "GamePrompt.h"
#include <regex>
#include "PlatformDefine.h"


using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;

using namespace CocosDenshion;

//----------------------  Password Layer ----------------------------
Layer* SetPasswordLayer::createLayer()
{
    auto layer = SetPasswordLayer::create();
    return layer;
}

bool SetPasswordLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    _uiPassWordLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("CommonLayer/ChangePw.json");
    addChild(_uiPassWordLayer);
    
    Size winSize = Director::getInstance()->getWinSize();
    _uiPassWordLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    _uiPassWordLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    
    // -- btn
    Button * btn_pw_confirm = (Button*)(Helper::seekWidgetByName(_uiPassWordLayer,"btn_pw_confirm"));
    btn_pw_confirm->addTouchEventListener(CC_CALLBACK_2(SetPasswordLayer::onBtnClicked,this));
    btn_pw_confirm->setTag(PW_CONFIRM);
    
    Button * btn_pw_cancel = (Button*)(Helper::seekWidgetByName(_uiPassWordLayer,"btn_pw_cancel"));
    btn_pw_cancel->addTouchEventListener(CC_CALLBACK_2(SetPasswordLayer::onBtnClicked,this));
    btn_pw_cancel->setTag(PW_CANCEL);
    
    Button * btn_close = (Button*)(Helper::seekWidgetByName(_uiPassWordLayer,"btn_close"));
    btn_close->addTouchEventListener(CC_CALLBACK_2(SetPasswordLayer::onBtnClicked,this));
    btn_close->setTag(PW_CANCEL);
    
    MSG_GP_R_LogonResult& LogonResult = PlatformLogic()->loginResult;
    
    Text*  label_userAcc = dynamic_cast<Text *>(Helper::seekWidgetByName(_uiPassWordLayer,"user_id_pw"));
    char userStr[15];
    sprintf(userStr, "%d",  LogonResult.dwUserID);
    label_userAcc->setString(userStr);
    
    _oldpw = (TextField*)Helper::seekWidgetByName(_uiPassWordLayer, "eb_current_pw");
    _newpw = (TextField*)Helper::seekWidgetByName(_uiPassWordLayer, "eb_new_pw");
    _confirmpw = (TextField*)Helper::seekWidgetByName(_uiPassWordLayer, "eb_confirm_pw");
    
    return true;
}


void SetPasswordLayer::onBtnClicked(Ref * pSender,Widget::TouchEventType type)
{
    Button *btn = (Button*)pSender;
    if(type==Widget::TouchEventType::ENDED && btn)
    {
        YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
        log("tag = %d", btn->getTag());
        switch (btn->getTag())
        {
            case PW_CONFIRM:
            {
                std::string str_oldpw = _oldpw->getStringValue();
                if (str_oldpw.empty())
                {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("原始密码不能为空。")); break;
                }
                if (_oldpw->getStringLength() > _oldpw->getMaxLength())
                {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("密码长度超过最大限制。")); break;
                }
                
                std::string str_newpw = _newpw->getStringValue();
                if (str_newpw.empty())
                {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("新密码不能为空。")); break;
                }
                if (_newpw->getStringLength() > _newpw->getMaxLength())
                {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("新密码长度超过最大限制。")); break;
                }
                
                std::string str_confirmpw = _confirmpw->getStringValue();
                if (str_confirmpw.empty())
                {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("确认密码不能为空。")); break;
                }
                if (_confirmpw->getStringLength() > _confirmpw->getMaxLength())
                {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("确认密码长度超过最大限制。")); break;
                }
                
                if (strcmp(str_oldpw.c_str(),LoadStringFromXML("save_password").c_str()) != 0)
                {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("原始密码错误。")); break;
                }
                if (strcmp(str_newpw.c_str(),str_confirmpw.c_str()) != 0)
                {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("密码与确认密码不同。")); break;
                }
                
                _userPswd = str_newpw;
                _userMD5Pswd = MD5_CTX::MD5String(str_newpw);
                
                MSG_GP_S_ChPassword ChPassword;
                ChPassword.dwUserID = PlatformLogic()->loginResult.dwUserID;
                strcpy(ChPassword.szMD5OldPass, LoadStringFromXML("saveMD5password").c_str());
                strcpy(ChPassword.szMD5NewPass, _userMD5Pswd.c_str());
                PlatformLogic()->sendData(MDM_GP_USERINFO, ASS_GP_USERINFO_UPDATE_PWD, &ChPassword, sizeof(ChPassword));
                PlatformLogic()->addEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_UPDATE_PWD, YZ_SOCKET_CALLBACK(SetPasswordLayer::modifyPasswordSelector, this));
            }
                break;
            case PW_CANCEL:
            {
                removeFromParentAndCleanup(true);
            }
                break;
            default:
                break;
        }
    }
}


bool SetPasswordLayer::modifyPasswordSelector(YZSocketMessage* socketMessage)
{
    if (HC_USERINFO_ACCEPT == socketMessage->messageHead.bHandleCode)	// 服务端已接受
    {
        SaveStringToXML("saveMD5password", _userMD5Pswd);
        SaveStringToXML("save_password", _userPswd);
        SaveXmlFlush();
        _userMD5Pswd.clear();
        
        auto prompt = GamePromptLayer::create();
        prompt->showPrompt(GBKToUtf8("密码修改成功。"));
        prompt->setCallBack([&]()
                            {
                                // 修改成功关闭密码修改界面
                                removeFromParentAndCleanup(true);
                            });
        
    }
    else if (HC_USERINFO_NOTACCEPT == socketMessage->messageHead.bHandleCode)	// 服务端未能接受
    {
        GamePromptLayer::create()->showPrompt(GBKToUtf8("密码修改失败。"));
    }
    else
    {
        
    }
    return true;
}




//----------------------  Email Layer ----------------------------


Layer* SetEmailLayer::createLayer()
{
    auto layer = SetEmailLayer::create();
    return layer;
}

bool SetEmailLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    _uiEmailLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("CommonLayer/SetEmail.json");
    addChild(_uiEmailLayer);
    
    Size winSize = Director::getInstance()->getWinSize();
    _uiEmailLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    _uiEmailLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    
    // -- btn
    Button * btn_mail_confirm = (Button*)(Helper::seekWidgetByName(_uiEmailLayer,"btn_mail_confirm"));
    btn_mail_confirm->addTouchEventListener(CC_CALLBACK_2(SetEmailLayer::onBtnClicked,this));
    btn_mail_confirm->setTag(MAIL_CONFIRM);
    
    Button * btn_mail_cancel = (Button*)(Helper::seekWidgetByName(_uiEmailLayer,"btn_mail_cancel"));
    btn_mail_cancel->addTouchEventListener(CC_CALLBACK_2(SetEmailLayer::onBtnClicked,this));
    btn_mail_cancel->setTag(MAIL_CANCEL);
    
    MSG_GP_R_LogonResult& LogonResult = PlatformLogic()->loginResult;
    
    Text*  label_userAcc = dynamic_cast<Text *>(Helper::seekWidgetByName(_uiEmailLayer,"user_id_mail"));
    char userStr[15];
    sprintf(userStr, "%d",  LogonResult.dwUserID);
    label_userAcc->setString(userStr);
    
    _userEmail = (TextField*)Helper::seekWidgetByName(_uiEmailLayer, "eb_email");
    
    return true;
}


void SetEmailLayer::onBtnClicked(Ref * pSender,Widget::TouchEventType type)
{
    Button *btn = (Button*)pSender;
    if(type==Widget::TouchEventType::ENDED && btn)
    {
        YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
        log("tag = %d", btn->getTag());
        switch (btn->getTag())
        {
            case MAIL_CONFIRM:
            {
                std::string str_email = _userEmail->getStringValue();
                if (str_email.empty())
                {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("绑定邮箱不能为空。")); break;
                }
                
                if (!verifyEmailAddress(str_email))
                {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("邮箱地址不合法。"));
                    break;
                }
                
                // 服务器发送绑定邮箱
                log("邮箱验证");
            }
                break;
            case MAIL_CANCEL:
            {
                removeFromParentAndCleanup(true);
            }
                break;
            default:
                break;
        }
    }
}

bool SetEmailLayer::verifyEmailAddress(const std::string& email)
{
    std::regex pattern("([0-9A-Za-z\\-_\\.]+)@([0-9a-z]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");
    if (std::regex_match(email, pattern))
    {
        return true;
    }
    return false;
}



//----------------------  Phone Layer ----------------------------


Layer* SetPhoneLayer::createLayer()
{
    auto layer = SetPhoneLayer::create();
    return layer;
}

bool SetPhoneLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    _uiPhoneLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("CommonLayer/BindPhone.json");
    addChild(_uiPhoneLayer);
    Size winSize = Director::getInstance()->getWinSize();
    _uiPhoneLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    _uiPhoneLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    
    // -- btn
    Button * btn_phone_confirm = (Button*)(Helper::seekWidgetByName(_uiPhoneLayer,"btn_phone_confirm"));
    btn_phone_confirm->addTouchEventListener(CC_CALLBACK_2(SetPhoneLayer::onBtnClicked,this));
    btn_phone_confirm->setTag(PHONE_CONFIRM);
    
    Button * btn_phone_cancel = (Button*)(Helper::seekWidgetByName(_uiPhoneLayer,"btn_phone_cancel"));
    btn_phone_cancel->addTouchEventListener(CC_CALLBACK_2(SetPhoneLayer::onBtnClicked,this));
    btn_phone_cancel->setTag(PHONE_CANCEL);
    
    Button * btn_close = (Button*)(Helper::seekWidgetByName(_uiPhoneLayer,"btn_close"));
    btn_close->addTouchEventListener(CC_CALLBACK_2(SetPhoneLayer::onBtnClicked,this));
    btn_close->setTag(PHONE_CANCEL);
    
    MSG_GP_R_LogonResult& LogonResult = PlatformLogic()->loginResult;
    
    Text*  label_userAcc = dynamic_cast<Text *>(Helper::seekWidgetByName(_uiPhoneLayer,"user_id_phone"));
    char userStr[15];
    sprintf(userStr, "%d",  LogonResult.dwUserID);
    label_userAcc->setString(userStr);
    
    _userPhone = (TextField*)Helper::seekWidgetByName(_uiPhoneLayer, "eb_phone");
    
    return true;
}

void SetPhoneLayer::onBtnClicked(Ref * pSender,Widget::TouchEventType type)
{
    Button *btn = (Button*)pSender;
    if(type==Widget::TouchEventType::ENDED && btn)
    {
        YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
        log("tag = %d", btn->getTag());
        switch (btn->getTag())
        {
            case PHONE_CONFIRM:
            {
                std::string str_phone = _userPhone->getStringValue();
                if (str_phone.empty())
                {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("绑定号码不能为空。")); break;
                }
                if (_userPhone->getStringLength() != _userPhone->getMaxLength())
                {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("请输入正确的11位手机号码。")); break;
                }
                
                // 服务器发送绑定邮箱
                log("手机绑定");
            }
                break;
            case PHONE_CANCEL:
            {
                removeFromParentAndCleanup(true);
            }
                break;
            default:
                break;
        }
    }
}



//----------------------  Trumpet Layer ----------------------------


Layer* TrumpetLayer::createLayer()
{
    auto layer = TrumpetLayer::create();
    return layer;
}

bool TrumpetLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    _uiTrumpetLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("CommonLayer/TrumpetMsg.json");
    addChild(_uiTrumpetLayer);
    Size winSize = Director::getInstance()->getWinSize();
    _uiTrumpetLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    _uiTrumpetLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    
    // -- btn
    Button * btn_phone_confirm = (Button*)(Helper::seekWidgetByName(_uiTrumpetLayer,"btn_send_confirm"));
    btn_phone_confirm->addTouchEventListener(CC_CALLBACK_2(TrumpetLayer::onBtnClicked,this));
    btn_phone_confirm->setTag(TRUMPET_CONFIRM);
    
    Button * btn_phone_cancel = (Button*)(Helper::seekWidgetByName(_uiTrumpetLayer,"btn_send_cancel"));
    btn_phone_cancel->addTouchEventListener(CC_CALLBACK_2(TrumpetLayer::onBtnClicked,this));
    btn_phone_cancel->setTag(TRUMPET_CANCEL);
    
    _userMsg = (TextField*)Helper::seekWidgetByName(_uiTrumpetLayer, "eb_trumpet_msg");
    
    return true;
}

void TrumpetLayer::onBtnClicked(Ref * pSender,Widget::TouchEventType type)
{
    Button *btn = (Button*)pSender;
    if(type==Widget::TouchEventType::ENDED && btn)
    {
        YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
        log("tag = %d", btn->getTag());
        switch (btn->getTag())
        {
            case TRUMPET_CONFIRM:
            {
                // 判断喇叭数量
                
                _TAG_BOARDCAST boardCast;
                memset(&boardCast,0,sizeof(_TAG_BOARDCAST));
                boardCast.nPropID =  Item_Trumpet_ID;
                boardCast.HoldCount = PlatformLogic()->loginResult.iTrumpet;
                boardCast.dwUserID = PlatformLogic()->loginResult.dwUserID;
                boardCast.iLength = sizeof(boardCast);
                memcpy(&boardCast.szUserName,&PlatformLogic()->loginResult.nickName,sizeof(PlatformLogic()->loginResult.nickName));
                
                std::string boardCastMsg = _userMsg->getString();
                strcpy(boardCast.szMessage,GBKToUtf8(boardCastMsg.c_str()));
                
                PlatformLogic()->sendData(MDM_GP_PROP, ASS_PROP_BIG_BOARDCASE, &boardCast, sizeof(boardCast));
                
                removeFromParentAndCleanup(true);
            }
                break;
            case TRUMPET_CANCEL:
            {
                removeFromParentAndCleanup(true);
            }
                break;
            default:
                break;
        }
    }
}

