#include "FishKingGameTableLogic.h"
#include "FishKingGameTableUICallback.h"
#include "FishKingMessageHead.h"
#include "YZNetExport.h"

namespace FishKing
{

	GameTableLogic::GameTableLogic(GameTableUICallback* tableUICallBack, BYTE deskNo, bool bAutoCreate) 
		: YZGameLogicBase(deskNo, PLAY_COUNT, bAutoCreate)
		, _tableUICallBack(tableUICallBack)
	{
		initParams();
	}

	GameTableLogic::~GameTableLogic()
	{
	}

	void GameTableLogic::enterGame()
	{
		loadDeskUsers();
		if(_mySeatNo == INVALID_DESKSTATION && !_autoCreate)
		{
			for(int i = 0; i < PLAY_COUNT; i++)
			{
				if(!_existPlayer[i])
				{
					sendUserSit(logicToViewSeatNo(i));
					break;
				}
			}
		}
		else
		{	//setSeatOffset(_mySeatNo);
			loadUsers();

			if(_mySeatNo != INVALID_DESKSTATION && _autoCreate)
			{
				sendGameInfo();
			}
		}
	}

	void GameTableLogic::loadUsers()
	{
		_MYSEATNO = _mySeatNo;
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
			if(_existPlayer[i] && pUser != nullptr)
			{
				_playerSitted[i] = true;
				_tableUICallBack->addUser(i, i == _mySeatNo);
				_tableUICallBack->dealSetUserMoney(i, pUser->i64Money);				
			}
		}
	}

	LLONG GameTableLogic::getUserMoney(BYTE seatNo)
	{
		UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(seatNo);
		return pUser->i64Money;
	}

	LLONG GameTableLogic::getUserCannonNum(BYTE seatNo)
	{
		return 0;
	}

/////////////////////////////////////////////////////////////////////////////////////////////
	void GameTableLogic::sendOpenFire(CMD_C_UserFire* pFire, INT size)
	{
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY , SUB_C_USER_FIRE, pFire, size);
	}

	void GameTableLogic::sendHitFish(BYTE seatNo, INT fishId, INT bulletId, BulletKind bulletKind, INT bulletMuriple)
	{
		CMD_C_CatchFish catch_fish;
		catch_fish.chair_id = seatNo;
		catch_fish.bullet_id = bulletId;
		catch_fish.bullet_kind = bulletKind;
		int anskind = (int)bulletKind;
		catch_fish.fish_id = fishId;
		catch_fish.bullet_mulriple = bulletMuriple;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, SUB_C_CATCH_FISH, (CHAR*)&catch_fish, sizeof(CMD_C_CatchFish));
	}

	void GameTableLogic::sendSweepFish(CMD_C_CatchSweepFish* pRet, INT size)
	{
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, SUB_C_CATCH_SWEEP_FISH, pRet, size);
	}

/////////////////////////////////////////////////////////////////////////////////////////////
	/*
	* @func: framework message function.
	* @info: they will be called when frame message come from base class.
	*/
/////////////////////////////////////////////////////////////////////////////////////////////
	void GameTableLogic::dealGameStartResp(BYTE bDeskNO)
	{
		YZGameLogicBase::dealGameStartResp(bDeskNO);
	}

	void GameTableLogic::dealGameEndResp(BYTE deskNo)
	{
		YZGameLogicBase::dealGameEndResp(deskNo);
	}

	void GameTableLogic::dealGamePointResp(void* object, INT objectSize)
	{
		YZGameLogicBase::dealGamePointResp(object, objectSize);

		if(objectSize != sizeof(MSG_GR_R_UserPoint)) return;	

		//TODO
		//MSG_GR_R_UserPoint * pUserPoint = (MSG_GR_R_UserPoint*)object;
	}

	void GameTableLogic::dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		//TODO
		bool isMe = (user->dwUserID == RoomLogic()->loginResult.pUserInfoStruct.dwUserID);
		if(isMe)
		{
			enterGame();
		}
		else
		{
			if(_playerSitted[userSit->bDeskStation])	 return;

			UserInfoStruct* UserInfo = _deskUserList->getUserByDeskStation(userSit->bDeskStation);
			_playerSitted[userSit->bDeskStation] = true;

			_tableUICallBack->addUser(userSit->bDeskStation, userSit->bDeskStation == _mySeatNo);
			_tableUICallBack->dealSetUserMoney(userSit->bDeskStation, UserInfo->i64Money);

		}
	}

	void GameTableLogic::dealQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& user)
	{
		YZGameLogicBase::dealQueueUserSitMessage(deskNo, user);

		if(_mySeatNo != INVALID_DESKSTATION)
		{
			enterGame();
		}
	}

	void GameTableLogic::dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		if(!_playerSitted[userSit->bDeskStation])	return;
		_playerSitted[userSit->bDeskStation] = false;

		//TODO
		_tableUICallBack->removeUser(userSit->bDeskStation, userSit->bDeskStation == _mySeatNo);
		if(_mySeatNo == INVALID_DESKSTATION)
		{
			//stop();
			_tableUICallBack->dealLeaveDesk();
		}
	}

	void GameTableLogic::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo)
	{
	}


	// 游戏状态
	void GameTableLogic::dealGameStationResp(void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(GameStation, objectSize, "GameStation size of error!");
		GameStation *pGameStation = (GameStation *)object; 
		
		CMD_S_GameConfig* pConfig = &pGameStation->GameConfig;
		_tableUICallBack->dealSetGameConfig(pConfig);

		CMD_S_GameStatus* pStatus = &pGameStation->GameStatus;
		_tableUICallBack->dealSetSceneStyle(pStatus->Scene_Style_Kind);
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			//显示桌上玩家炮火倍数
			_tableUICallBack->dealSetUserCannonNum(i, pStatus->iUserFireBase[i],pStatus->iUserFireCell);
		}
	}

	// 游戏消息（游戏的主体消息来自这里）
	void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		if(MDM_GM_GAME_NOTIFY != messageHead->bMainID)
		{
			YZLOG("onGameMessage MainID is error.");
		}

		switch (messageHead->bAssistantID)
		{
		case SUB_S_GAME_CONFIG://设置游戏参数
			{
				CHECK_SOCKET_DATA(CMD_S_GameConfig, objectSize, "CMD_S_GameConfig size of error!");
				CMD_S_GameConfig* pConfig = (CMD_S_GameConfig*)object;

				_tableUICallBack->dealSetGameConfig(pConfig);
			}
			break;
		case SUB_S_FISH_TRACE://生成新鱼的轨迹
			{
				_tableUICallBack->dealAddFishInfo(object, objectSize);
			}
			break;
		case SUB_S_FISH_GROUP_TRACE://生成鱼群的轨迹
			{
				CHECK_SOCKET_DATA(CMD_S_GroupFishTrace, objectSize, "CMD_S_GroupFishTrace size of error!");
				CMD_S_GroupFishTrace* pTrace = (CMD_S_GroupFishTrace*)object;

				_tableUICallBack->dealAddGroupFishInfo(pTrace);
			}
			break;
		case SUB_S_EXCHANGE_FISHSCORE:
			break;
		case SUB_S_USER_FIRE://玩家射击
			{
				CHECK_SOCKET_DATA(CMD_S_UserFire, objectSize, "CMD_S_UserFire size of error!");
				CMD_S_UserFire* pFire = (CMD_S_UserFire*)object;

				_tableUICallBack->dealGameFire(pFire, false);
			}
			break;
		case SUB_S_CATCH_FISH://抓到鱼
			{
				CHECK_SOCKET_DATA(CMD_S_CatchFish, objectSize, "CMD_S_CatchFish size of error!");
				CMD_S_CatchFish* pCatch = (CMD_S_CatchFish*)object;

				_tableUICallBack->dealCatchFish(pCatch);
			}
			break;
		case SUB_S_BULLET_ION_TIMEOUT://能量炮失效
			{
				CHECK_SOCKET_DATA(CMD_S_BulletIonTimeout, objectSize, "CMD_S_BulletIonTimeout size of error!");
				CMD_S_BulletIonTimeout* pTimeOut = (CMD_S_BulletIonTimeout*)object;

				_tableUICallBack->dealBulletTimeOut(pTimeOut->chair_id);	
			}
			break;
		case SUB_S_LOCK_TIMEOUT://定屏炸弹失效时间
			{
				_tableUICallBack->dealLockFishTimeOut();
			}
			break;
		case SUB_S_CATCH_SWEEP_FISH://抓到特殊鱼（炸弹，鱼王）
			{
				CHECK_SOCKET_DATA(CMD_S_CatchSweepFish, objectSize, "CMD_S_CatchSweepFish size of error!");
				CMD_S_CatchSweepFish* pSweep = (CMD_S_CatchSweepFish*)object;

				_tableUICallBack->dealCatchSweepFish(pSweep);

			}
			break;
		case SUB_S_CATCH_SWEEP_FISH_RESULT://抓到特殊鱼结果
			{
				CHECK_SOCKET_DATA(CMD_S_CatchSweepFishResult, objectSize, "CMD_S_CatchSweepFishResult size of error!");
				CMD_S_CatchSweepFishResult* pSweep = (CMD_S_CatchSweepFishResult*)object;

				_tableUICallBack->dealCatchSweepFishResult(pSweep);

			}
			break;
		case SUB_S_CATCH_SWEEP_FISH_RESULT_EX://广播通知所有玩家捕到大鱼
			{
				CHECK_SOCKET_DATA(CMD_S_CatchSweepFishResultEx, objectSize, "CMD_S_CatchSweepFishResultEx size of error!");
				CMD_S_CatchSweepFishResultEx* pResult = (CMD_S_CatchSweepFishResultEx*)object;
				
				_tableUICallBack->dealCatchSweepFishResultEx(pResult);
			}
			break;
		case SUB_S_SWITCH_SCENE://切换场景，5种鱼阵
			{
				CHECK_SOCKET_DATA(CMD_S_SwitchScene, objectSize, "CMD_S_SwitchScene size of error!");
				CMD_S_SwitchScene* pScene = (CMD_S_SwitchScene*)object;

				_tableUICallBack->dealSwitchScene(pScene);
			}
			break;
		case SUB_S_HIT_FISH_LK:
		case SUB_S_SCENE_END:
			break;
		case S_C_SUPER_CONFIG_SIG://是否超端玩家
			{
				CHECK_SOCKET_DATA(S_C_SUPER_CONFIG, objectSize, "S_C_SUPER_CONFIG size of error!");
				S_C_SUPER_CONFIG* pSuper = (S_C_SUPER_CONFIG*)object;

				_tableUICallBack->dealSetAdminConfig(pSuper->bSuperUser);
			}
			break;
		case SUB_S_STOCK_OPERATE_RESULT://查询库存，抽水
			{
				CHECK_SOCKET_DATA(CMD_S_StockOperateResult, objectSize, "CMD_S_StockOperateResult size of error!");
				CMD_S_StockOperateResult* pStock = (CMD_S_StockOperateResult*)object;

				_tableUICallBack->dealStockOperateResult(pStock);
			}
			break;
		case S_C_UPDATE_ONLINEUSER_INFO_SIG://更新在线玩家信息
			{
				CHECK_SOCKET_DATA(S_C_ON_LINE_USER, objectSize, "S_C_ON_LINE_USER size of error!");
				S_C_ON_LINE_USER* pOnline = (S_C_ON_LINE_USER*)object;

				if (pOnline->bStart) _users.clear(); 

				_users.push_back(pOnline->user_info_);
			}
			break;
		case S_C_END_UPDATE_ONLINEUSER_INFO_SIG://更新在线玩家信息列表
			{
				_tableUICallBack->dealUpdateOnlinePlayerList(_users);
			}
			break;
		case S_C_UPDATE_CONTROL_INFO_SIG://更新控制概率信息
			{
				CHECK_SOCKET_DATA(S_C_UPDATE_CONTROL_INFO, objectSize, "S_C_UPDATE_CONTROL_INFO size of error!");
				S_C_UPDATE_CONTROL_INFO* pControl = (S_C_UPDATE_CONTROL_INFO*)object;

				_tableUICallBack->dealUpdateContorlList(pControl);
			}
			break;
		case S_C_UPDATE_SPECIAL_INFO_SIG://更新控制特殊鱼信息
			{
				CHECK_SOCKET_DATA(S_C_UPDATE_SPECIAL_INFO, objectSize, "S_C_UPDATE_SPECIAL_INFO size of error!");
				S_C_UPDATE_SPECIAL_INFO* pSpecial = (S_C_UPDATE_SPECIAL_INFO*)object;

				_tableUICallBack->dealUpdateSpecialList(pSpecial);
			}
			break;
		case S_C_SWITCH_INFO_SIG://控制开关信息
			{
				CHECK_SOCKET_DATA(S_C_SWITCH_INFO, objectSize, "S_C_SWITCH_INFO size of error!");
				S_C_SWITCH_INFO* pSwitch = (S_C_SWITCH_INFO*)object;

				_tableUICallBack->dealSetSwitchInfo(pSwitch);
			}
			break;
		case S_C_UPDATE_FISH_NAME_SIG://更新可控制鱼名称
			{
				CHECK_SOCKET_DATA(S_C_FISH_NAME, objectSize, "S_C_FISH_NAME size of error!");
				S_C_FISH_NAME* pName = (S_C_FISH_NAME*)object;

				_tableUICallBack->dealUpdateFishName(pName);
			}
			break;
		default:
			break;
		}
	}

	void GameTableLogic::initParams()
	{
		memset(_playerSitted, 0, sizeof(_playerSitted));
	}

	/*
	* refresh member before every game start.
	*/
	void GameTableLogic::refreshParams()
	{

	}

	//查询奖池操作
	void GameTableLogic::sendStockOperate(BYTE operate_code)
	{
		CMD_C_StockOperate stock_operate;
		stock_operate.operate_code = operate_code;

		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, SUB_C_STOCK_OPERATE, &stock_operate, sizeof(stock_operate));
	}
	void GameTableLogic::setSeatOffset(BYTE seatNo)
    {
	    _seatOffset = -seatNo;
    }

	void GameTableLogic::sendForceQuit()
	{
		if (RoomLogic()->connected())
		{
			YZGameLogicBase::sendForceQuit();
		}
		_tableUICallBack->dealLeaveDesk();
	}

	void GameTableLogic::sendUserUp()
	{
		if (RoomLogic()->connected())
		{
			YZGameLogicBase::sendUserUp();
		}
		else
		{
			_tableUICallBack->dealLeaveDesk();
		}
	}
}
