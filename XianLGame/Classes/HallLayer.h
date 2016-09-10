#ifndef __HALL_LAYER_H__
#define __HALL_LAYER_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "PlatformDefine.h"

using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;

//layer tag
#define LAYER_HALL 100
#define LAYER_SHARE 101
#define LAYER_AWARD 102
#define LAYER_MESSAGEBOX 110
#define LAYER_SIGN 111

// Button Tags
#define  BTN_STORE 1001
#define  BTN_BANK  1002
#define  BTN_HELP 1003
#define  BTN_SHARE 1004
#define  BTN_EXIT 1005
#define  BTN_GLODADD 1006


#define  BTN_FREE 1007
#define  BTN_TREASURE 1008
#define  BTN_SET 1009
#define  BTN_TRUMPET 1010

#define  BTN_SHARE 1011

#define  BTN_SHARE_EXIT 1012


#define  BTN_HEAD 3011
#define  LABEL_NICK 3012
#define  LABEL_GOLD 3013

#define  IMAGE_MSG_BOX 1014


//game tag

#define GAME_PDK 2001
#define GAME_CSMJ 2002

#define GAME_HZMJ 5001
#define GAME_BYDR 5002

struct MSG_GP_S_USER_SHARE
{
    int		dwUserID;
    int		nListType;
};

struct MSG_GP_R_USER_SHARE_REWARD
{
    int		dwUserID;
    int		m_iVuale;
    int		m_day;
};

class HallLayer : public YZPlatformEventLayer, MoneyChangeNotify
{
public:
    Widget* _uiHallLayer;
    Widget* _uiHallTop;
    Widget* _uiMessageBox;
    Widget* _uiShareLayer;
    Button* personal;
    Text* user_goldInfo;
    Text* label_up_msg;
    Text* label_userName;
    bool _isOpen;
    
public:
    static void createScene();
    static Layer * craeteHallLayer();
    static void returnHallLayer();
    
public:
    virtual bool init() override;
    
    void onBtnClicked(Ref * pSender,Widget::TouchEventType type);
    void gameSelect(Ref * pSender,Widget::TouchEventType type);
    
    // 刷新人物头像
    void setHallHeadPic();
    
    void modifyName(std::string name);
    // 滚动消息
    void setMsgRollAction();
    // 房间列表处理消息
    virtual void onHandleRoomListMessage() override;
    // 游戏列表处理消息
    virtual void onHandleGameListMessage() override;
    // 监听玩家购买金币消息
    virtual void walletChanged(LLONG money) override;
    
    void requestGameTimerCallBack(float dt);
    
    bool shareEventBack(YZSocketMessage* socketMessage);
    
    void sendShareSuc();
    
    void showLoadAward();
    
    CREATE_FUNC(HallLayer);
    
private:
    PageView* _pageViewGames;		// 游戏列表
    Button* _currentSelectedGame;	// 列表按钮
    
    Button*	 _leftArrow;
    Button*	 _rightArrow;
    
    ui::Widget* _gamelistLayer;
    
    int _nRoomType;
    
    EditBox* _editPhone;
    Point pos[10];
    //////////////////////////////////////////////////////////////////////////
    Widget* _pSignLayer;		//签到
    Widget* _pPhoneBindLayer;	//绑定电话
    char	_szPhone[32];		//电话
    char	_szUserId[32];		//用户ID
    EditBox* _editUserId;
    bool canSign;			// 能否签到
    int signDays;			// 签到日期
    ImageView* sign_sel;		// 签到标志
    // 界面回调接口
public:
    // pageView拖动回调函数
    void pageViewMoveCallBack(Ref* pSender, PageView::EventType type);
    
    // 列表按钮点击回调函数
    void enterGameEventCallBack(Ref* pSender, Widget::TouchEventType type);
    
    void arrowCallBack(Ref* pSender, Widget::TouchEventType type, INT buttonTag);
    
    // 功能接口
private:
    // 创建房间列表
    void createGameList();
    // 创建房间
    void createGame(int _gTag1,int _gTag2,int _gTag3,int _gTag4);
    // 创建房间元素
    ImageView* createGameItem(int _gTag);
    
    void updateArrow(PageView* pageView);
    
    bool isDigitStr(const char* cstr);	//手机号码格式判断
    
    Label * roomLabel;
    
    void reRequest(float dt);
    
    float waitTime;
public:
    // 手机返回键监听回调函数
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent);
    
    void setOpenSave(bool isOpen);
};


#endif 
