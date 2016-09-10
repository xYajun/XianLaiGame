#ifndef __YZ_FishKing_GameTableUI_H__
#define __YZ_FishKing_GameTableUI_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "YZNetExport.h"
#include "FishKingGameTableLogic.h"
#include "FishKingGameTableUICallback.h"
#include "FishKingMessageHead.h"
#include "FishKingGamePlayer.h"
#include <vector>

USING_NS_CC;

using namespace cocos2d;
using namespace cocostudio;
using namespace std;
using namespace ui;

namespace FishKing
{
	struct catchResultInfo
	{
		LLONG userGoldMoney;							//玩家当前捕鱼获得的金币
		INT numberOfKindFIsh[FISH_KIND_COUNT];			//玩家当前每种鱼捕到的数量
	};

	static const FLOAT		PI					=	3.14159f;
	static const INT		BACKGROUND_ZORDER	=	0;
	static const INT		FISH_ZORDER			=	10;
	static const INT		NOTICE_ZORDER		=	50;
	static const INT  		BULLET_ZORDER		=	15;
	static const INT		WATER_ZORDER		=	9;
	static const INT		PLAYER_ZORDER		=	210;
	static const INT		MAX_ZORDER			=	280;

	class GameFishManager;
	class SceneFishTrace;

	class GameTableUI : public YZ::YZGameUIBase, public GameTableUICallback
	{

	private:
		BYTE						_deskIndex;
		bool						_bAutoCreate;
		bool						_isAllowFire;
		bool						_isAutoFire;
		bool						_isReadyLock;
		INT							_iBullet_id;
		Vec2						_touchPoint;
		ImageView*					_currentBg;
		ui::Layout*						_tableUi;
		CC_SYNTHESIZE(INT,			_lockFishId, LockRishId);

	public:
		Vec2						_fpCircleFishCenter[2];	//环形鱼群中心点

	private:
		GameFishManager*			_fishManager;
		GameTableLogic*				_tableLogic;
		SceneFishTrace*				_fishTrace;
		//GameNotice*					_notic;
		Label*                      _text;
		Sprite*						_NoticeBG;
		GamePlayer*					_players[PLAY_COUNT];
		catchResultInfo*			_resultInfo;			//玩家捕鱼结果信息
		CMD_S_SwitchScene			_switchScene;

	public:
		GameTableUI();
		virtual ~GameTableUI();

		virtual void onEnter();
		virtual void onExit();
		virtual void onEnterTransitionDidFinish() override;

		static GameTableUI* create(BYTE bDeskIndex, bool autoCreate);
		bool init(BYTE bDeskIndex, bool bAutoCreate);
		bool initGameUI();
		void initToolBarData();
		void asyncBuildData();
		void delayedLoadAction(float dt);

	public:
		virtual void addUser(BYTE seatNo, bool bMe) override;									//添加玩家
		virtual void removeUser(BYTE seatNo, bool bMe) override;								//移除玩家
		virtual void dealSetUserMoney(BYTE seatNo, LLONG value) override;						//设置显示玩家金币
		virtual void dealSetUserCannonNum(BYTE seatNo, INT value ,INT valueCell) override;						//设置玩家炮火威力
		virtual void dealSetGameConfig(CMD_S_GameConfig* gConfig) override;						//设置子弹速度等参数
		virtual void dealSetSceneStyle(SceneStyle Scene) override;								//游戏场景id
		virtual void dealGameFire(CMD_S_UserFire* pFire, bool bMeFire) override;				//收到服务端开火消息
		virtual void dealCatchFish(CMD_S_CatchFish*	pFish) override;							//捕到鱼消息
		virtual void dealAddFishInfo(void* object, WORD objectSize) override;					//添加单鱼信息（长度可能为多条鱼）
		virtual void dealAddGroupFishInfo(CMD_S_GroupFishTrace* pFish) override;				//添加鱼群信息
		virtual void dealCatchSweepFish(CMD_S_CatchSweepFish* pSweep) override;					//抓捕到特殊鱼（鱼王，炸弹）通知
		virtual void dealCatchSweepFishResult(CMD_S_CatchSweepFishResult* pResult) override;	//抓捕到特殊鱼产生的结果（满屏死亡或同类死亡）
		virtual void dealCatchSweepFishResultEx(CMD_S_CatchSweepFishResultEx* pResult) override;//抓到大鱼系统广播通知
		virtual void dealSwitchScene(CMD_S_SwitchScene* pScene) override;						//切换场景
		virtual void dealLockFishTimeOut() override;											//被定海神针定住的鱼恢复运动
		virtual void dealBulletTimeOut(BYTE seatNo) override;									//取消能量炮

		////////////////////////////////////////////////////////////////////
		//超端
		////////////////////////////////////////////////////////////////////
		virtual void dealSetAdminConfig(bool bAdmin)  override;									//是否超端用户
		virtual void dealStockOperateResult(CMD_S_StockOperateResult* pStock)  override;		//奖池操作结果
		virtual void dealUpdateOnlinePlayerList(std::vector<OnLineUserInfo> users) override;	//更新在线玩家列表
		virtual void dealUpdateContorlList(S_C_UPDATE_CONTROL_INFO* info) override;				//更新控制列表信息
		virtual void dealUpdateSpecialList(S_C_UPDATE_SPECIAL_INFO* pSpecial) override;			//更新特殊鱼控制列表信息
		virtual void dealSetSwitchInfo(S_C_SWITCH_INFO* pSwitch) override;						//控制开关
		virtual void dealUpdateFishName(S_C_FISH_NAME* pName) override;							//更新控制鱼的名字



		////////////////////////////////////////////////////////////////////

	public:
		virtual void dealGameStart() override;
		virtual void dealLeaveDesk() override;
		virtual void onDisconnect();
		void returnPlatform();

	public:
		//发送子弹碰到鱼消息
		void sendHitFish(BYTE bSeatNo, INT fishId , INT bulletId , BulletKind bulletKind , INT  bulletMuriple);
		//播放抓到鱼后奖励特效
		void playAwardFishAnimation(BYTE bSeatNo, LLONG money, LLONG userMoney, Fish* fish);
		//切换场景动画
		void switchScene();

	public:
		//结算退出
		void gameSettlementAndExit();

	private:
		//点击事件
		virtual bool onTouchBegan(Touch *touch, Event *unused_event);
		virtual void onTouchMoved(Touch *touch, Event *unused_event);
		virtual void onTouchEnded(Touch *touch, Event *unused_event);
		//自动射击
		void setAtuoFire(float dt);
		//长按连续射击
		void continuousFire(float dt);
		//触摸屏幕射击
		void openFireByTouch(const Vec2& touchPoint);
		//超时不开火自动退出
		void gameOutTime(bool bShow);

	private:
		//计时器
		void timing(Node* node, int dt, std::function<void()> callFunc);
		//显示通知信息
		void showNoticeMessage(const std::string& message);

	private:
		bool isValidSeat(BYTE seatNo);

	public:
		//手机返回键监听回调函数
		virtual void onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent) override;

	};
}


#endif // __YZ_FishKing_GameTableUI_H__
