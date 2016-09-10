#include "PersonalLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalUnits.h"
#include "StoreLayer.h"
#include "GamePrompt.h"
#include "CommonLayer.h"
#include "HallLayer.h"

#define Name_Defaul_Pic				"HallLayer/res/head_default.png"
#define Name_Local_Head_Pic			"PersonalHead/local_head_"



using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;


Layer* PersonalLayer::createLayer()
{
    PersonalLayer* pRet = new PersonalLayer();
    
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

bool PersonalLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    _uiPersonalLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("PersonLayer/PersonLayer_1.json");
    addChild(_uiPersonalLayer);
    _uiPersonalLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    _uiPersonalLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    _uiPersonalLayer->setTag(PERSONALLAYER);
    
    
    Button* btn_exit = (Button*)(Helper::seekWidgetByName(_uiPersonalLayer,"btn_close"));
    btn_exit->addTouchEventListener(CC_CALLBACK_2(PersonalLayer::onBtnClicked,this));
    btn_exit->setTag(BTN_PERSONALLAYER_EXIT);
    
    initPanel_Personal();
    return true;
}




void PersonalLayer::initPanel_Personal()
{
    // personal property values
    MSG_GP_R_LogonResult& LogonResult = PlatformLogic()->loginResult;
    
    
    Button* btn_selgirl = (Button*)(Helper::seekWidgetByName(_uiPersonalLayer,"btn_girl"));
    btn_selgirl->addTouchEventListener(CC_CALLBACK_2(PersonalLayer::onBtnSelClicked,this));
    btn_selgirl->setTag(BTN_SEL_GIRL);
    
    Button* btn_selboy = (Button*)(Helper::seekWidgetByName(_uiPersonalLayer,"btn_boy"));
    btn_selboy->addTouchEventListener(CC_CALLBACK_2(PersonalLayer::onBtnSelClicked,this));
    btn_selboy->setTag(BTN_SEL_BOY);
    
    
    img_sexgirl = (ImageView*)(Helper::seekWidgetByName(_uiPersonalLayer,"girl_selected"));
    img_sexgirl->setTag(IMG_SEX_GIRL);
    img_sexboy = (ImageView*)(Helper::seekWidgetByName(_uiPersonalLayer,"boy_selected"));
    img_sexboy->setTag(IMG_SEX_BOY);
    
    // 获取并设置角色性别
    _isBoy = LogonResult.bBoy;
    reSetRoleSex(_isBoy);
    
    btn_head = dynamic_cast<Button *>(Helper::seekWidgetByName(_uiPersonalLayer,"btn_head"));
    btn_head->addTouchEventListener(CC_CALLBACK_2(PersonalLayer::headBtnClicked,this));
    
    // 定义头像
    setPersonalHeadPic();
    
    // 定义ID
    Text* label_userid = dynamic_cast<Text *>(Helper::seekWidgetByName(_uiPersonalLayer,"Label_id"));
    char userStr[15];
    sprintf(userStr, "%d",  LogonResult.dwUserID);
    label_userid->setString(userStr);
    
    
    // 定义账号
    label_name = dynamic_cast<Text *>(Helper::seekWidgetByName(_uiPersonalLayer,"Label_name"));
    std::string str = LogonResult.szName;
    label_name->setString(GBKToUtf8(str.c_str()));
    
    //user editBix
    ImageView* box_acc = static_cast<ImageView*>(Helper::seekWidgetByName(_uiPersonalLayer, "infor_nick"));
    auto editBoxSize = box_acc->getSize();
    
    // top edit
    std::string pNormalSprite = "shuru.png";
    tf_nickname = ui::EditBox::create(editBoxSize, ui::Scale9Sprite::create(pNormalSprite));
    tf_nickname->setFontColor(Color3B(128, 128, 128));
    tf_nickname->setPosition(box_acc->getPosition());
    tf_nickname->setAnchorPoint(Vec2(0,0.5));
    tf_nickname->setFontSize(25);
    tf_nickname->setReturnType(EditBox::KeyboardReturnType::DONE);
    tf_nickname->setDelegate(this);
    tf_nickname->setScale(0.5);
    _uiPersonalLayer->addChild(tf_nickname,15);;
    tf_nickname->setText(GBKToUtf8(LogonResult.nickName));
    
    btn_save = dynamic_cast<Button *>(Helper::seekWidgetByName(_uiPersonalLayer,"btn_sure"));
    btn_save->addTouchEventListener(CC_CALLBACK_2(PersonalLayer::onBtnClicked,this));
    btn_save->setTag(BTN_SAVE);
    
    
    
}


void PersonalLayer::setPersonalHeadPic()
{
    MSG_GP_R_LogonResult& LogonResult = PlatformLogic()->loginResult;
    _headImgId =  LogonResult.bLogoID;
    char name[60];
    memset(name, 0, sizeof(name));
    sprintf(name, "%s%02d%s", "PersonalHead/local_head_",_headImgId,".png");
    btn_head->loadTextureNormal(Name_Defaul_Pic);
    btn_head->loadTexturePressed(Name_Defaul_Pic);
    btn_head->loadTextureNormal(name);
    btn_head->loadTexturePressed(name);
}



void PersonalLayer::onBtnClicked(Ref * pSender,Widget::TouchEventType type)
{
    Button *btn = (Button*)pSender;
    
    if(type==Widget::TouchEventType::ENDED && btn)
    {
        YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
        log("tag = %d", btn->getTag());
        switch (btn->getTag())
        {
            case BTN_PERSONALLAYER_EXIT:
            {
                removeFromParentAndCleanup(true);
            }
                break;
            case BTN_SAVE:
            {
                std::string str_nickname =(std::string)(tf_nickname->getText());
                if (str_nickname.empty())
                {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("昵称不能为空。")); break;
                }
                if ( str_nickname.length() < 6)
                {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("昵称长度至少为6英文字符或2个汉字。")); break;
                }
                else
                {
                    MSG_GP_R_LogonResult LogonResult = PlatformLogic()->loginResult;
                    LogonResult.bLogoID = _headImgId;
                    LogonResult.bBoy=_isBoy;
                    strcpy(LogonResult.nickName,Utf8ToGB(str_nickname.c_str()));
                    //strcpy(LogonResult.nickName,str_nickname.c_str());
                    PlatformLogic()->sendData(MDM_GP_USERINFO,ASS_GP_USERINFO_UPDATE_DETAIL,&LogonResult,sizeof(MSG_GP_R_LogonResult));
                    PlatformLogic()->addEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_UPDATE_DETAIL, YZ_SOCKET_CALLBACK(PersonalLayer::modifyRoleInforSelector, this));
                }
                
            }
                break;
            default:
                break;
        }
    }
}

void PersonalLayer::onBtnSelClicked(Ref * pSender,Widget::TouchEventType type)
{
    Button *btn = (Button *)pSender;
    if(type==Widget::TouchEventType::ENDED && btn)
    {
        switch (btn->getTag())
        {
            case BTN_SEL_GIRL:
            {
                // 传消息给服务器 修改性别
                MSG_GP_R_LogonResult LogonResult = PlatformLogic()->loginResult;
                if (_isBoy != 0)
                {
                    _isBoy = 0;
                    reSetRoleSex(_isBoy);
                }
                
            }
                break;
            case BTN_SEL_BOY:
            {
                MSG_GP_R_LogonResult LogonResult = PlatformLogic()->loginResult;
                if (_isBoy != 1)
                {
                    _isBoy= 1;
                    reSetRoleSex(_isBoy);
                }
            }
                break;
            default:
                break;
        }
    }
}


// 设置角色性别
void PersonalLayer::reSetRoleSex(bool isboy)
{
    if (isboy)
    {
        img_sexgirl->setVisible(false);
        img_sexboy->setVisible(true);
    }
    else
    {
        img_sexgirl->setVisible(true);
        img_sexboy->setVisible(false);
    }
}


// 修改
bool PersonalLayer::modifyRoleInforSelector(YZSocketMessage* socketMessage)
{
    if (HC_USERINFO_ACCEPT == socketMessage->messageHead.bHandleCode)	// 服务端已接受
    {
        std::string str_nickname = (std::string)tf_nickname->getText();
        MSG_GP_R_LogonResult& LogonResult  =  (PlatformLogic()->loginResult);
        strcpy(LogonResult.nickName,Utf8ToGB(str_nickname.c_str()));
        LogonResult.bLogoID = _headImgId;
        LogonResult.bBoy=_isBoy;
        ((HallLayer*)(this->getParent()))->modifyName(str_nickname);
        ((HallLayer*)(this->getParent()))->setHallHeadPic();
        removeFromParentAndCleanup(true);
        
    }
    else if (HC_USERINFO_NOTACCEPT == socketMessage->messageHead.bHandleCode)	// 服务端未能接受
    {
        GamePromptLayer::create()->showPrompt(GBKToUtf8("个人资料修改失败。"));
    }
    return true;
}


void PersonalLayer::headBtnClicked(Ref * pSender,Widget::TouchEventType type)
{
    Button *btn = (Button*)pSender;
    if(type==Widget::TouchEventType::ENDED && btn)
    {
        
        _headList = static_cast<ListView*>(Helper::seekWidgetByName(_uiPersonalLayer, "ListView_3"));
        _headList->setGravity(cocos2d::ui::ListView::Gravity::CENTER_HORIZONTAL);
        _headList->setItemsMargin(0.0f);
        _headList->setClippingEnabled(true);
        char buffer[128];
        for (int i=1;i<=18;i++)
        {
            
            auto _uiBtnLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("PersonLayer/HeadCell.json");
            Button* headButton = (Button*)(Helper::seekWidgetByName(_uiBtnLayer,"Button_1"));
            headButton->addTouchEventListener(CC_CALLBACK_2(PersonalLayer::onHeadPicClicked,this));
            headButton->setTag(LOCAL_HEAD_PIC_TAG + i);
            memset(buffer, 0, sizeof(buffer));
            sprintf(buffer, "%s%02d%s", Name_Local_Head_Pic,i,".png");
            headButton->loadTextureNormal(buffer);
            headButton->loadTexturePressed(buffer);
            _headList->addChild(_uiBtnLayer);
            
            
        }
        
    }
}

void PersonalLayer::onHeadPicClicked(Ref * pSender,Widget::TouchEventType type)
{
    Button *btn = (Button*)pSender;
    if(type==Widget::TouchEventType::ENDED && btn)
    {
        int tag = btn->getTag();
        _headImgId = tag - LOCAL_HEAD_PIC_TAG;
        
        char name[60];
        sprintf(name,"%s%02d%s", Name_Local_Head_Pic,_headImgId,".png");
        btn_head->loadTextureNormal(name);
        btn_head->loadTexturePressed(name);
    }
}

void  PersonalLayer::editBoxReturn(EditBox* editBox)
{
}

void  PersonalLayer::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    if (tf_nickname == editBox)
    {
        memset(_regNick, 0, 32);
        memcpy(_regNick, text.c_str(), text.length());
        log("_editNick");
    }
}





