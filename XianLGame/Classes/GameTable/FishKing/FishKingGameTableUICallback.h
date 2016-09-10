#ifndef __HN_FishKing_GameTableUICallback_H__
#define __HN_FishKing_GameTableUICallback_H__

#include "YZBaseType.h"
#include "YZNetExport.h"
#include "FishKingMessageHead.h"

/************************************************************************/
/* 类型：回调接口                                                        */
/* 功能：声明牌桌逻辑在收到消息时调用界面的函数接口                        */
/************************************************************************/

namespace FishKing
{
	class GameTableUICallback 
	{
	public:
		virtual void addUser(BYTE seatNo, bool bMe) = 0;											//添加玩家
		virtual void removeUser(BYTE seatNo, bool bMe) = 0;											//移除玩家
		virtual void dealSetUserMoney(BYTE seatNo, LLONG value) = 0;								//设置玩家金币数目
		virtual void dealSetUserCannonNum(BYTE seatNo, INT value,INT valueCell) = 0;								//设置玩家炮火等级

		///////////////////////////////////////////////////////////////////
		virtual void dealSetGameConfig(CMD_S_GameConfig* gConfig) = 0;								//接收游戏配置
		virtual void dealSetSceneStyle(SceneStyle Scene) = 0;										//游戏场景id
		virtual void dealGameFire(CMD_S_UserFire* pFire, bool bMeFire) = 0;							//玩家开火
		virtual void dealCatchFish(CMD_S_CatchFish*	pFish) = 0;										//抓到普通鱼消息
		virtual void dealAddFishInfo(void* object  , WORD objectSize) = 0;							//添加普通鱼
		virtual void dealAddGroupFishInfo(CMD_S_GroupFishTrace* pFish) = 0;							//捕到带盘鱼王，出现小鱼阵
		virtual void dealCatchSweepFish(CMD_S_CatchSweepFish* pSweep) = 0;							//玩家捕到大鱼
		virtual void dealCatchSweepFishResult(CMD_S_CatchSweepFishResult* pResult) = 0;				//玩家捕到大鱼结果
		virtual void dealCatchSweepFishResultEx(CMD_S_CatchSweepFishResultEx* pResult) = 0;			//捕到大鱼系统通知
		virtual void dealSwitchScene(CMD_S_SwitchScene* pScene) = 0;								//切换场景
		virtual void dealLockFishTimeOut() = 0;														//被定海神针定住的鱼恢复运动
		virtual void dealBulletTimeOut(BYTE seatNo) = 0;											//取消能量炮


		////////////////////////////////////////////////////////////////////
		virtual void dealGameStart() = 0;
		virtual void dealLeaveDesk() = 0;
		virtual void onDisconnect() = 0;

		////////////////////////////////////////////////////////////////////
		//超端
		////////////////////////////////////////////////////////////////////
		virtual void dealSetAdminConfig(bool bAdmin) = 0;											//是否超端用户
		virtual void dealStockOperateResult(CMD_S_StockOperateResult* pStock) = 0;					//奖池操作结果
		virtual void dealUpdateOnlinePlayerList(std::vector<OnLineUserInfo> users) = 0;			//更新在线玩家列表
		virtual void dealUpdateContorlList(S_C_UPDATE_CONTROL_INFO* info) = 0;						//更新控制列表信息
		virtual void dealUpdateSpecialList(S_C_UPDATE_SPECIAL_INFO* pSpecial) = 0;					//更新特殊鱼控制列表信息
		virtual void dealSetSwitchInfo(S_C_SWITCH_INFO* pSwitch) = 0;									//控制开关
		virtual void dealUpdateFishName(S_C_FISH_NAME* pName) = 0;									//更新控制鱼的名字

	};
};

#endif //__FishKing_GameTableUICallback_H__