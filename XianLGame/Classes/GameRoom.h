#ifndef __GameRoom_SCENE_H__
#define __GameRoom_SCENE_H__

#include "YZNetExport.h"
#include "YZUIExport.h"

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

using namespace cocostudio;
using namespace ui;

class GameRoom : public YZRoomEventLayer
{
private:
    PageView* _pageViewRooms;		// 房间列表
    Button* _currentSelectedRoom;	// 列表按钮
    
    Button*	 _leftArrow;
    Button*	 _rightArrow;
    
    ui::Widget* _uiRoomLayer;
    
    
public:
    CREATE_FUNC(GameRoom);
    
public:
    GameRoom();
    virtual ~GameRoom();
    
public:
    virtual bool init() override;
    
    virtual void onExit() override;
    // 返回大厅按钮回调函数
    void returnEventCallBack();
    bool onTouchBegan(Touch *touch, Event *unused_event);
    void onTouchEnded(Touch *touch, Event *unused_event);
    
    // 界面回调接口
public:
    // pageView拖动回调函数
    void pageViewMoveCallBack(Ref* pSender, PageView::EventType type);
    
    // 列表按钮点击回调函数
    void enterRoomEventCallBack(Ref* pSender, Widget::TouchEventType type);
    
    void arrowCallBack(Ref* pSender, Widget::TouchEventType type, INT buttonTag);
    
    //设置房间信息
    void setGameInfo(const long& lGameTag);
    
    // 功能接口
private:
    // 创建房间列表
    void createRoomList();
    // 创建房间
    void createRoom(ComRoomInfo* roomInfo1, ComRoomInfo* roomInfo2, ComRoomInfo* roomInfo3);
    // 创建房间元素
    Layout* createRoomItem(ComRoomInfo* roomInfo);
    
    void updateArrow(PageView* pageView);
    
    // 定时器回调函数
public:
    // 更新房间人数信息定时器
    void updateRoomPeopleTimerCallBack(float dt);
    // 连接房间超时定时器
    void enterRoomOutTimerTimerCallBack(float dt);
    
    // 基类虚接口
public:
    // 网络断开消息
    virtual void onDisConnect() override;
    // 房间连接成功
    virtual void onConnectMessage(bool success) override;
    // 房间登录成功
    virtual void onLoginMessage(bool success, UINT dwErrorCode) override;
    // 房间登录完成
    virtual void onLoginFinishMessage() override;
    // 用户进入房间
    virtual void onUserComeMessage(UserInfoStruct* user) override;
    // 用户离开房间
    virtual void onUserLeftMessage(UserInfoStruct* user) override;
    // 排队用户坐下
    virtual void onQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& users) override;
    // 用户坐下
    virtual void onUserSitMessage(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
    // 用户站起
    virtual void onUserUpMessage(MSG_GR_R_UserUp * userUp, UserInfoStruct* user) override;
    // 用户断线
    virtual void onUserCutMessage(INT dwUserID, BYTE bDeskNO, BYTE	bDeskStation) override;
    // 房间其他消息
    virtual bool onRoomFrameMessage(NetMessageHead* messageHead, void* object, INT objectSize) override;
    
public:

    ComRoomInfo* roomInfo;
};

#endif // __GameRoom_SCENE_H__
