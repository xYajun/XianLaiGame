#ifndef __LOGIN_LAYER_H__
#define __LOGIN_LAYER_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "YZNetExport.h"

using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;

// 按钮tag
#define GUEST_LOGIN_BTN 1002
#define LOGIN_BTN 1003
#define WXLOGIN_BTN 1004
#define CHECK_REGISTER_BTN 1005
#define ACCOUNT_LAYER_CLOSE_BTN 1006
#define REGISTER_LAYER_CLOSE_BTN 1007
#define LOOK_XY_BTN 1008

// 层tag
#define  ACCOUNT_LOGIN_LAYER 2001
#define  REGISTER_LAYER 2002
#define	 LOGIN_LAYER 2003
#define	 USEINFO_LAYER 2004
#define	 FINDPW_LAYER 2005
#define	 FINDPW_PHONE_LAYER 2006
#define	 FINDPW_EMAIL_LAYER 2007
#define  REGISTER_CHECKNICK_LAYER 2008

// 图片tag
#define  REMEBER_PASD 3001
#define  CHECK_REMEBER_PASD 3002
#define  USER_PROTOCOL 3003
#define  CHECK_USER_PROTOCOL 3004
#define  USE_INFO 3005
#define	 NICK_HEAD_IMG 3006

#define LOCAL_HEADSEX_PIC_TAG 100001
#define SELECT_HEAD_PIC_TAG 30000

#define SAVE_USERNAME "save_name"
#define SAVE_PASSWORD "save_password"
#define SAVE_MD5PASSWORD "saveMD5password"
#define SAVE_ISGUEST  "save_isguest"
#define SAVE_USERID  "save_uid"
#define SAVE_ISRMBPSED "save_isrmbpswd"

#define DEFAULT_USERNAME "test000"
#define DEFAULT_PASSWORD "123456"

#define Word_Loading		GBKToUtf8("正在连接服务器，请稍后...")
#define Word_Reging			GBKToUtf8("正在注册......")
#define Word_Empty_Name		GBKToUtf8("账号不能为空！")
#define Word_Empty_Pass		GBKToUtf8("密码不能为空！ ")
#define Word_Wrong_Name		GBKToUtf8("登陆名字错误")
#define Word_Wrong_Pass		GBKToUtf8("用户密码错误")
#define Word_Logined		GBKToUtf8("帐号已经登录")
#define Word_Down		    GBKToUtf8("低于最小版本,必须升级!")
#define Word_Up		        GBKToUtf8("高于最大版本 不支持该版本!")
#define Word_Wrong_GUEST_REG		GBKToUtf8("游客注册失败")
#define Word_Wrong_GUEST_BIND		GBKToUtf8("注册失败，用户名已存在")
#define Word_Wrong_USER_REG		GBKToUtf8("注册失败，用户名已存在")
#define Word_Success_USER_BIND     GBKToUtf8("恭喜您，注册成功！")
#define Word_Success_USER_REG     GBKToUtf8("恭喜您，账号注册成功！")

#define Word_Mail_Sending		GBKToUtf8("正在提交......")

class LoginLayer : public YZPlatformEventLayer
{
public:
    static cocos2d::Scene* createScene();
    
    static void returnLogin();
    
    virtual bool init();
    
    static LoginLayer * _GameLoginLayer;
    static LoginLayer* getInstance();

    CREATE_FUNC(LoginLayer);
    
    void onBtnClicked(Ref * pSender,Widget::TouchEventType type);
        
    void creatInitLoginLayer();
    
public:
    void loginPlatform();
    
    void guestLogin();
    
    void userRegister();
    
    void checkName();
    
    virtual void update(float dt);
    
    //接收服务器数据
    virtual void onHandleConnectMessage(bool result) override;
    
    virtual void onHandleLoginMessage(bool result, UINT dwErrorCode) override;
    
    // 平台注册
    virtual void onHandleUserRegisterMessage(void* registerStruct, UINT ErrorCode) override;
    
    // 网络断线
    virtual void onHandleDisConnect() override;
    
    bool isDigitStr(const char* cstr);//判断是否是纯数字
    
    void buttonActionEnd(Node* sender,int data);
    
public:
    // 手机返回键监听回调函数
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent);
    
private:
    ui::Widget* _uiInitLoginLayer;
    std::string image_content;
    ui::CheckBox * agreeCB;
    Sprite* _uiHallTop;
    EditBox* _editLoginName;
    EditBox* _editLoginPassword;
    EditBox* _editRegisterName;
    EditBox* _editRegisterPswd;
    EditBox* _editRegisterCPswd;
    EditBox* _editRegisterNick;
    EditBox* _editPhone;
    EditBox* _editEmail;
    
    ListView* _headList;
    ImageView* sex_img;
    Sprite *selectImg;
    MSG_GP_S_LogonByNameStruct _loginByName;
    char _szPass[32];
    char _regName[32];
    char _regPswd[32];
    char _regNick[32];
    int  _regSex;
    int _headImgId;
    
    long m_checkCode;
    UINT m_count;
    bool _isLogin;
    bool _isQuickLogin;
    bool _isRegister;
    bool _isChechName;
    bool _isKickNickImg;
    bool _isLoginSucess;
    bool _isNeedLogin;
    float times;
};

#endif 
