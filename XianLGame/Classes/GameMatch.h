#ifndef GameMatch_h__
#define GameMatch_h__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "StoreLayer.h"
#include "YZNetExport.h"
#include "YZUIExport.h"
#include <vector>

USING_NS_CC;

using namespace cocostudio;
using namespace ui;
using namespace YZ;

class GameMatch : public YZRoomEventLayer, MoneyChangeNotify
{
private:
    ComRoomInfo* _roomInfo;
    ui::Button* _button_bm;
    ui::Widget* _tableWidget;
    ui::Widget* _topWidget;
    bool _isApply;
    
public:
    static void createMatch(ComRoomInfo* roomInfo);
    static GameMatch* create(ComRoomInfo* roomInfo) ;
public:
    GameMatch(ComRoomInfo* roomInfo);
    virtual ~GameMatch();
    
    virtual bool init();
    virtual void onExit() override;
    
public:
    
private:
    void menuClickedCallback(Ref* pRef,  Widget::TouchEventType touchtype);
    bool onApplyRet(YZSocketMessage* socketMessage);
    bool onExitRet(YZSocketMessage* socketMessage);
    void leaveDesk();
public:
    void callEverySecond(float delta);
    // 网络检测回调定时器函数
    void gameNetworkCheckCallBack(float dt);
    // 返回大厅按钮回调函数
    void returnEventCallBack(Ref* pSender, Widget::TouchEventType type);
    
    // 打开商店按钮回调函数
    void rechargeEventCallBack(Ref* pSender, Widget::TouchEventType type);
    
    // 快速开始游戏按钮回调函数
    void quickStartCallBack(Ref* pSender, Widget::TouchEventType type);
    
    // 监听玩家选择房间的消息
    void onBtnClicked(Ref* pSender, Widget::TouchEventType type);
    // 监听玩家购买金币消息
public:
    virtual void walletChanged(LLONG money) override;
    
public:
    // 手机返回键监听回调函数
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent);
    
public:
    // 断线重连
    virtual void onDisConnect();
    // 房间连接成功
    // 	virtual void onConnectMessage(bool result) override;
    // 	// 用户进入房间
    // 	virtual void onUserComeMessage(UserInfoStruct* user) override;
    // 	// 用户离开房间
    // 	virtual void onUserLeftMessage(UserInfoStruct* user) override;
    // 	// 用户坐下
    // 	virtual void onUserSitMessage(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
    // 	// 用户站起
    // 	virtual void onUserUpMessage(MSG_GR_R_UserUp * userUp, UserInfoStruct* user) override;
    // 	// 房间其他消息
    // 	virtual bool onRoomFrameMessage(NetMessageHead* messageHead, void* object, INT objectSize) override { return false; }
};

#endif // GameDesk_h__