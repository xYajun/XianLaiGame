#ifndef __FishKing_GamePlayer_H__
#define __FishKing_GamePlayer_H__

#include "cocostudio/CocoStudio.h"
#include "FishKingGameFishManager.h"
#include "FishKingMessageHead.h"
#include "ui/CocosGUI.h"
#include <string>

using namespace ui;
using namespace std;
using namespace YZ;

USING_NS_CC;

namespace FishKing
{
	class GameTableUI;
	class GameBullet;

	class GamePlayer : public YZLayer
	{
	public:
		enum CANNON_TYPE{
			DoubleTube = 0,
			ThreeTube
		};
	public:
		ui::Text*		AtlasLabel_Money;
		ui::Text*		AtlasLabel_CannonNum;
		Layout*         Layout_Table;
		ImageView*		Image_Battery;
		ImageView*		Image_MoneyBox;
		Sprite*			Sprite_Cannon;
		//ImageView*        Sprite_Cannon;
		ImageView*		Image_Card;
		ImageView*		Image_Lock;
		Sprite*			Sprite_Bingo;
        Button*         btn_add;
        Button*         btn_sub;

		GamePlayer();
		virtual ~GamePlayer();

	public:
		static GamePlayer* createGamePlayer(BYTE seatNo, Layout* Panel_Table, GameFishManager* fishManager);
		bool init(BYTE seatNo, Layout* Panel_Table, GameFishManager* fishManager);

	public:
		//设置各种参数
		void setConfig(CMD_S_GameConfig* gConfig);
		//设置炮台是否可见（此位置是否有玩家）
		void setBatteryVisible(bool bVisible);
		//显示玩家金币
		void setUserMoney(const LLONG money);
		//显示玩家鱼炮等级
		void setCannonNum(const INT cannonNum);

		void setCannonNumCell(const INT cannonNumCell);
		//设置鱼炮类型
		void setCannonType(CANNON_TYPE type);
		//设置子弹类型
		void setBulletKind(BulletKind kind);
		//设置是否能量炮
		void setIsEnergyCannon(bool isEnergyCannon);
		//设置是否锁定鱼
		void setIsLockFish(bool bLock, INT fishId);
		//设置bingo转盘
		void setBingo(LLONG money);
        
        void setCannonAddOrSubVisible(int userId,bool visible);

	public:
		//获取当前玩家是否存在
		bool batteryIsVisible();
		//获取当前鱼炮位置
		Vec2 getCannonPoint();
		//获取玩家金币位置，为了做奖励回收动画
		Vec2 getMoneyPoint();
		//获取鱼网范围
		FLOAT getBulletNetRadiusById(BYTE id);
		//能否开火
		bool isFireEnable();
		//锁定指示气泡线条
		void showLockIndicatorLine(float dt);


	public:
		//显示开火
		void gameFire(FLOAT degree, BYTE seatNo, INT bulletId, BulletKind bulletKind, INT bulletMulriple, INT lockFishId, bool isRobot = false);
		//播放鱼炮开火动画
		void playCannonFireAction();
		void menuEventCallBack(Ref* pSender, Widget::TouchEventType type);

	private:
		CC_SYNTHESIZE(bool,			_isMe, IsMe);
		CC_SYNTHESIZE(bool,			_isRobot, Robot);
		CC_SYNTHESIZE(BYTE,			_seatNo, SeatNo);
		CC_SYNTHESIZE(INT,			_userId, UserID);
		CC_SYNTHESIZE(LLONG,		_currentMoney, CurrentMoney);
		CC_SYNTHESIZE(INT,			_min_bullet_multiple, MinMultiple);
		CC_SYNTHESIZE(INT,			_max_bullet_multiple, MaxMultiple);
		CC_SYNTHESIZE(INT,			_currentFirepower, Firepower);
		CC_SYNTHESIZE(GameTableUI*,	_tableUI, GameTable);
		CC_SYNTHESIZE(INT,			_bulletNum, BulletNum);
		CC_SYNTHESIZE(bool,			_bLock, Lock_Fish);
		CC_SYNTHESIZE(INT,			_lockFishId, LockFishId);
		CC_SYNTHESIZE(BulletKind,	_currentBulletKind, Bullet_Kind);

		INT							_bulletSpeed[BULLET_KIND_COUNT];
		INT							_netRadius[BULLET_KIND_COUNT];
		std::string					bulletName[BULLET_KIND_COUNT];
		bool						_bEnergyCannon;
		GameFishManager*			_fishManager;
		Armature*					_cannonArmature;
		CANNON_TYPE					_cannonType;
		INT                         _currentFireCell;

	public:
		std::vector<GameBullet*>	_bulletList;
		std::vector<Sprite*>		_bubbleList;
	};

}


#endif // __FishKing_GamePlayer_H__
