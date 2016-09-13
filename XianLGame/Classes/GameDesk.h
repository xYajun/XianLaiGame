#ifndef GameDesk_h__
#define GameDesk_h__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "StoreLayer.h"
#include "YZNetExport.h"
#include "YZUIExport.h"
#include <vector>
#include "GlobalUnits.h"
USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "network/HttpResponse.h"
#include "network/HttpClient.h"
USING_NS_CC;
using namespace network;
#endif
#define DESK_PASSWD "set_desk_passwd"
#define IS_RM_DESK_PWD "set_is_rm_desk_pSd"

#define DESK_PASSWD_TAG 9000001
#define IS_RM_DESK_PWD_TAG 9000002

using namespace cocostudio;
using namespace ui;
using namespace YZ;

enum DeskListUI_type
{
    DESKNUM_TAG				= 53,             //桌号
    MULTIPLE_TAG			= 55,            //倍率
    GOLD_TAG				= 56,                //最小携带金钱
    PROGRESS_TAG			= 57,            //人数进度条
    PEOPLENUM_TAG			= 58,           //人数
    DESK_STATE_IMAGE_TAG	= 61,		//牌桌状态
};

struct PropInfo{
    int			nPropID;						//道具ID号
    std::string		szPropName;					//道具名称
    int			attribAction;					//操作属性
    int			attribValue;					//取值属性
    int			iPrice;							//道具价格
    int			iVipPrice;						//VIP道具价格
    int         count;                          //数量
};


enum button_tag
{
    BTN_LEFTDESK = 1001,
    BTN_RIGHTDESK
};

//桌子信息
typedef struct tagDeskInfo
{
    INT deskID[2];               //桌号
    INT multiple[2];             //倍率
    INT goldMin[2];              //最小携带金钱
    INT goldMax[2];              //最大携带金钱
    INT peopleValue[2];          //桌子当前人数
    INT peopleMax[2];            //桌子最大人数
    /*bool isOwner;
     INT  ownerID;*/
    bool isOwner[2];
    INT  ownerID[2];
    bool lockTag[2];				//加密状态
}DeskInfo;

class GameDesk : public YZRoomEventLayer, MoneyChangeNotify
{
private:
    bool					_quicklyGame;			// 自动入桌
    INT						_roomPeopleCount;		// 房间人数
    ListView*				_deskItems;				// 桌子列表
    DeskInfo*				_currentSelectedDesk;	// 当前选中的桌子
    bool                    _isSelect;              // 牌桌被选中标记
    INT                     _selectIndex;           // 当前被选中牌桌的序号
    Color3B                 _saveColor;             // 当前牌桌列表颜色暂存
    ComRoomInfo*			_roomInfo;				// 房间信息
    TextSprite*				_textSpriteUserChips;
    bool					_isHaveDesk;			// 是否显示桌子
    Text*                   label_roomCount;        //房卡数量
    
    std::vector<DeskInfo*>	_deskinfos;
    
    Button*  _quickStart;
    Sprite*  _deskListBG;
    
    Button*  _ruleBtn;
    Button*  _setBtn;
    
    ui::Widget* _uiRoomRule;
    TextField*  _tfSetPwd;
    TextField*  _tfInputPwd;
    
    ui::ImageView*  _headImg;
    
    BYTE _seatNo;
    bool _iMySet;
    bool _iBR;
    bool _iCanEnter;
    BYTE _myDeskNo;
    INT  _currentSelectDeskNo;
    
    Layout* uiSetRoomPwdLayer;
    
    
    MSG_GR_DeskStation _curDeskStation;
public:
    static void createDesk(ComRoomInfo* roomInfo, bool isHaveDesk);
    static GameDesk* create(bool isHaveDesk) ;
public:
    GameDesk(bool isHaveDesk);
    virtual ~GameDesk();
    
    virtual bool init();
    virtual void onExit() override;
    virtual void onEnter() override;
    
public:
    void addDesk(ComRoomInfo* romInfo);
    void clearDesksData();
    // 创建房间菜单
    void createMenu();
    // 创建牌桌列表
    void createDeskList();
    
    void clickHeadImage();
private:
    // 蹇�鍔犲叆娓告垙
    bool quicklyJoinGame();
    bool quicklyJoinGame(DeskInfo* desk,int isleft);
    
    // 离开桌子
    void leaveDesk();
    
    void updateRoomPeople(INT count);
    
    void updateDeskInfo(Widget* deskItem, int isLeft, bool isOwner, int ownerId, bool lockTag);
    //void updateDeskInfo(Widget* deskItem, int isLeft, bool isOwner, int ownerId,bool uptag);
    // 网络检测回调定时器函数
    void gameNetworkCheckCallBack(float dt);
    
public:
    void selectedItemEventCALLBACK(cocos2d::Ref *pSender, ListView::EventType type);
    
    // 返回大厅按钮回调函数
    void returnEventCallBack(Ref* pSender, Widget::TouchEventType type);
    
    //游戏规则
    void ruleEventCallBack(Ref* pSender, Widget::TouchEventType type);
    
    //设置房间密码
    void setEventCallBack(Ref* pSender, Widget::TouchEventType type);
    
    void exitEventCallBack(Ref* pSender, Widget::TouchEventType type);
    
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
    virtual void onDisConnect() override;
    // 房间连接成功
    virtual void onConnectMessage(bool result) override;
    // 用户进入房间
    virtual void onUserComeMessage(UserInfoStruct* user) override;
    // 用户离开房间
    virtual void onUserLeftMessage(UserInfoStruct* user) override;
    // 用户坐下
    virtual void onUserSitMessage(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
    // 用户站起
    virtual void onUserUpMessage(MSG_GR_R_UserUp * userUp, UserInfoStruct* user) override;
    // 房间其他消息
    virtual bool onRoomFrameMessage(NetMessageHead* messageHead, void* object, INT objectSize) override;
    
    virtual void onBackRoomMessage(MSG_GR_BACKROOM_STATUS* data) override;
    //使用房卡消息
    virtual void onUseRoomCardMessage(MSG_PROP_S_OP_ROOMCARD * data) override;
    //创建房间消息
    virtual void onCreateBackRoomMessage(MSG_BACKROOM_INFO * data) override;
    //查询房间消息
    virtual void onQueryBakcRoomMessage(MSG_QueryBACKROOM_RSP_INFO * data , BYTE code) override;
    //virtual void onGameDeskMessage(MSG_GR_DeskStation * desk) override;;
    virtual void onQueryPropRoomCardInfo(MSG_PROP_S_QUERY_ROOMCARD * data , BYTE bHandleCode) override;

    virtual void onQueryPropKindMessage(MSG_PROP_S_KIND_ROOMCARD * data , BYTE bHandleCode ) override;

    //购买房卡
    virtual void onBuyRoomCardMessage(MSG_PROP_S_BUY_ROOMCARD * data) override;

    virtual void onQueryGameRecordMessage() ;
    
    void bgSliderEvent(cocos2d::Ref *pSender, SliderEventType type);
    void soundSliderEvent(cocos2d::Ref *pSender, SliderEventType type);
    void btnClickEventCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype);
private:
    void initOpenRoomDesk();        //初始化界面
    void enterRoomDesk();           //加入房间

    void queryBackRoomInfoFromService();
    
    void queryGameRecordFromServer(); //查询游戏记录
    
    void showBuyRoomCardLayer(bool isShow = true);
    
    void changeBuyRoomCardCount(int count);
    
    void buyRoomCard();     //购买房卡
    
    void createRoom();  //创建房间
    
    void onBtnClickCallBack(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
    
    void inputRoomIdCallBack(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
    
    void btnChooseRoomCountCallBack(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);  //创建房间选择房间次数回调
    
    void btnBuyRoomCardCallBack(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);  //购买房卡回调
    
    void selectedRoomCountStateEvent(cocos2d::Ref* pSender, CheckBoxEventType type);
    
    void selectedRoomZMStateEvent(cocos2d::Ref* pSender, CheckBoxEventType type);
    
    void queryBackRoom();
    
    void userSit(INT destNo , BYTE seatNo);
    
        Button* createRoomBtn;  //创建房间按钮
    
    ui::Widget*  _uiCreateRoomLayer;
    ui::Widget*  _uiEnterGameLayer;
    ui::Widget*  _uiGetRoomCardLayer;

    ui::Text* _roomNumTxt[6];     //房间次数
    ui::Text* _buyRoomCardCountLabel;           //够买房卡数量
    ui::Text* _buyRoomCardMoneyCount;           //够买前金币数量
    ui::Text* _afterBuyRoomCardMoneyCount;           //够买后金币数量

    std::vector<CheckBox *> roomCountCBoxVec;       //创建房间时局数选择框
    
    std::vector<CheckBox *> roomZMCBoxVec;       //创建房间时扎码选择框
    
    int nowRoomNumIndex;

    void hideCreateRoomLayer();
    
    int _selectRoomCount;           //房间创建局数
    
    int _selectRoomZMCount;         //房间创建扎码数
    
    int _selectBuyRoomCardCount;    //购买房卡数量
    
    CHAR roomPwd[6];
    
    EventListenerTouchOneByOne* listener;
    
    Layer* waitLayer;
    
    std::map<int , PropInfo> userPorpInfos;  //Key ： 房间次数  Value ： 房间信息
    
    Widget* _settingWidget;
    Button* _btnMusic;
    Button* _btnSound;
    ui::Slider * bgSlider;
    ui::Slider * soundSlider;
    
    
};

#endif // GameDesk_h__